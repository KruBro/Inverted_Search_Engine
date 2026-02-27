# 📋 CHANGELOG — Inverted Search Engine

> A full record of every bug identified, its root cause, the original problematic code, and the fix applied. Claude (AI) was used as a reviewing and documentation assistant throughout this project.

---

## Version History

| Version | Description |
|---|---|
| `v1.0` | Initial implementation — core data structures, hash table, and menu loop |
| `v1.1` | Bug fixes across four critical areas (documented below) |
| `v1.2` | Incremental update architecture introduced |
| `v1.3` | Performance optimizations — sorted chains, single-pass insert, snprintf |
| `v1.4` | Dynamic string allocation, segfault fix, punctuation stripping, prefix search, input validation, automated test target |

---

## 🐛 Bug #1 — Infinite Loop in `create_database.c`

**File:** `create_database.c`  
**Version Fixed:** v1.1  
**Severity:** 🔴 Critical — causes the program to hang on any word that appears more than once in the same file.

### Root Cause

When a word was already present in the hash table **and** a sub-node for the current file already existed, the code correctly incremented `sTemp->wordcount` and called `break` — but this only broke the **inner** `while(sTemp)` loop. The **outer** `while(mTemp)` loop had no corresponding `break`, so after finding and updating the word, the loop would continue iterating indefinitely.

### Before (Broken)

```c
while(mTemp)
{
    if(strcmp(mTemp->word, input_word) == 0)
    {
        sNode *sTemp = mTemp->sLink, *sPrev = NULL;
        while(sTemp)
        {
            if(strcmp(sTemp->file_name, temp->file_name) == 0)
            {
                (sTemp->wordcount)++;
                break;      // ✅ breaks inner loop
                            // ❌ outer loop still continues!
            }
        }
        // ← No break here. Outer loop keeps running forever.
    }
    else
    {
        mPrev = mTemp;
        mTemp = mTemp->mLink;
    }
}
```

### After (Fixed)

```c
while(mTemp)
{
    if(strcmp(mTemp->word, input_word) == 0)
    {
        // ... handle word ...
        break;  // ✅ correctly exits the outer loop too
    }
    else
    {
        mPrev = mTemp;
        mTemp = mTemp->mLink;
    }
}
```

---

## 🐛 Bug #2 — Pass-by-Value & Stack `free()` in `update_database.c`

**File:** `update_database.c`, `main.c`  
**Version Fixed:** v1.1  
**Severity:** 🔴 Critical — causes silent data loss and a crash on exit.

### Root Cause (Part A) — Pass by Value

`update_database` received `Flist *head` (a copy). New nodes added inside the function were lost the moment it returned because the caller's pointer was never updated.

### Root Cause (Part B) — Freeing a Stack Array

After collecting filenames into a VLA (`char *fileHolder[fileCount]`), the original code called `free(fileHolder)`. A VLA is stack-allocated — calling `free()` on it is undefined behavior and causes a crash.

### Before (Broken)

```c
update_database(hash_t, head, fileHolder, fileCount);  // ❌ by value
free(fileHolder);                                       // ❌ stack memory
```

### After (Fixed)

```c
update_database(hash_t, &head, fileHolder, fileCount); // ✅ by pointer

for(int i = 0; i < fileCount; i++)
    free(fileHolder[i]);   // ✅ only free the strdup'd strings
// fileHolder itself lives on the stack — no free needed
```

---

## 🐛 Bug #3 — Duplicate File Detection in `validation.c`

**File:** `validation.c`  
**Version Fixed:** v1.1  
**Severity:** 🟠 High — allows the same file to be indexed multiple times, inflating all word counts.

### Root Cause

`compare()` used a nested loop where the inner pointer `temp1` was advanced but the `strcmp` always compared against the outer pointer `temp`. The last node was never checked, allowing it to be inserted as a duplicate.

### Before (Broken)

```c
while(temp)
{
    Flist *temp1 = temp->link;
    while(temp1)
    {
        if(strcmp(temp->file_name, fname) == 0)  // ❌ always checks temp, not temp1
            return DUPLICATE;
        temp1 = temp1->link;
    }
    temp = temp->link;
}
```

### After (Fixed)

```c
while(temp)
{
    if(strcmp(temp->file_name, fname) == 0)  // ✅ checks every node including the last
        return DUPLICATE;
    temp = temp->link;
}
```

---

## 🐛 Bug #4 — Full Re-index on Every Update in `update_database.c`

**File:** `update_database.c`  
**Version Fixed:** v1.2  
**Severity:** 🟡 Medium — O(all files) cost on every update instead of O(new files only).

### Root Cause

The original update wiped the entire hash table and re-read every file from scratch on every update call.

### Before (Inefficient)

```c
free_hash_table(arr);
initialize_hashTable(arr);
create_database(arr, *head);    // re-indexes ALL files every time
```

### After (Incremental)

```c
// Record where the existing list ends
Flist *lastNode = *head;
while (lastNode->link != NULL) lastNode = lastNode->link;

// ... add new files ...

// Index only from the new entries onward
Flist *startNode = (lastNode == NULL) ? *head : lastNode->link;
create_database(arr, startNode);
```

---

## ⚡ Optimization #1 — Sorted mNode Chain in `create_database.c`

**Version:** v1.3  
**Impact:** Reduces average chain traversal from O(C) to O(C/2) with early-exit on miss.

mNode chains are now kept in alphabetical order. Traversal stops as soon as `mTemp->word > input_word`, meaning a miss is detected without scanning to NULL.

```c
// Before: always scanned to NULL even if word couldn't possibly be there
while(mTemp) { ... mTemp = mTemp->mLink; }

// After: stop early when we've passed the insertion point
while(mTemp)
{
    cmp = strcmp(mTemp->word, input_word);
    if(cmp >= 0) break;   // found match (==0) or passed it (>0)
    mPrev = mTemp;
    mTemp = mTemp->mLink;
}
```

---

## ⚡ Optimization #2 — Single-Pass `insert_at_last` in `flist_utils.c`

**Version:** v1.3  
**Impact:** Halves Flist traversal cost on every file insertion.

The old code called `compare()` (one full pass), then traversed to the tail again (a second full pass). Both are now done in a single loop.

```c
// Before: two O(F) traversals
if(compare(*head, fname) == DUPLICATE) ...  // pass 1
while(temp->link) temp = temp->link;        // pass 2

// After: one O(F) traversal — duplicate check and tail find combined
while(temp->link)
{
    if(strcmp(temp->file_name, fname) == 0) { free(new); return FAILURE; }
    temp = temp->link;
}
```

---

## ⚡ Optimization #3 — `snprintf` with Running Offset in `display_database.c` / `save_database.c`

**Version:** v1.3  
**Impact:** Filename string building drops from O(n²) to O(n).

`strcat` scans to the end of the destination string on every call — O(n) per call, O(n²) total when called in a loop. Replaced with `snprintf` + a running position pointer.

```c
// Before: O(n²)
strcat(all_files, sTemp->file_name);

// After: O(1) per write
pos += snprintf(all_files + pos, sizeof(all_files) - pos, "%s", sTemp->file_name);
```

---

## 🐛 Bug #5 — Segfault on Exit in `hash_t_utils.c`

**File:** `hash_t_utils.c`, `main.h`, `create_database.c`, `flist_utils.c`  
**Version Fixed:** v1.4  
**Severity:** 🔴 Critical — program crashes on exit when `free_hash_table` is called.

### Root Cause

The structs `mNode`, `sNode`, and `Flist` originally stored strings in fixed `char[]` arrays (stack memory inside the struct). The code in `hash_t_utils.c` called `free(mPrev->word)` on these, which is undefined behavior — you cannot `free()` memory that was not heap-allocated. This caused an immediate segfault on exit.

### Fix — Dynamic String Allocation (v1.4)

All string fields were changed from fixed arrays to heap-allocated pointers via `strdup`. Every creation point uses `strdup`; every free point now correctly frees the string before freeing the node.

```c
// Before — fixed arrays in structs (main.h):
typedef struct mainNode { char word[20];      ... } mNode;
typedef struct subNode  { char file_name[20]; ... } sNode;
typedef struct Node     { char file_name[20]; ... } Flist;

// After — heap-allocated pointers (main.h):
typedef struct mainNode { char *word;      ... } mNode;  /* strdup'd */
typedef struct subNode  { char *file_name; ... } sNode;  /* strdup'd */
typedef struct Node     { char *file_name; ... } Flist;  /* strdup'd */
```

```c
// Before — create_database.c (strcpy into fixed array):
strcpy(new_mainNode->word, input_word);
strcpy(new_subNode->file_name, temp->file_name);

// After — create_database.c (strdup to heap):
new_mainNode->word     = strdup(input_word);
new_subNode->file_name = strdup(temp->file_name);
```

```c
// Before — hash_t_utils.c (freeing wrong memory, only freeing mNode):
mNode *mPrev = mTemp;
mTemp = mTemp->mLink;
free(mPrev->word);   // ❌ crashed — word was char[20], not heap
free(mPrev);

// After — hash_t_utils.c (free string, then node — for both sNode and mNode):
sNode *sPrev = sTemp;
sTemp = sTemp->subLink;
free(sPrev->file_name); // ✅ heap string freed first
free(sPrev);

mNode *mPrev = mTemp;
mTemp = mTemp->mLink;
free(mPrev->word);      // ✅ heap string freed first
free(mPrev);
```

---

## ✨ Feature — Punctuation Stripping (`files_utils.c`)

**Version:** v1.4  
**File added:** `files_utils.c`

A new `strip_punctuation()` function is called in `create_database.c` after every `fscanf` read. It strips all non-alpha characters except mid-word apostrophes, so `"hello,"` and `"hello"` index as the same token.

```c
// Smart apostrophe rule: keep only if surrounded by letters on both sides
// it's  → it's   (apostrophe kept — alpha on both sides)
// 'hi'  → hi     (apostrophe stripped — not surrounded by alpha)
```

A `continue` guard was also added to skip tokens that become empty after stripping (e.g. `"---"` → `""`), preventing empty-string entries from corrupting the index.

---

## ✨ Feature — Prefix Search in `search_database.c`

**Version:** v1.4

Search was updated from exact matching to **prefix matching** using `strncasecmp`. The entire bucket chain is now scanned (not stopped at first match), so searching `"the"` returns `"the"`, `"there"`, `"they"`, and any other word with that prefix.

```c
// Before — exact match only, stopped at first hit:
while(mTemp && strcasecmp(mTemp->word, word) != 0)
    mTemp = mTemp->mLink;

// After — prefix match, scans full chain for all matches:
while(mTemp != NULL)
{
    if(strncasecmp(mTemp->word, word, search_len) == 0)
    {
        // ... print all results for this match ...
    }
    mTemp = mTemp->mLink;
}
```

---

## ✨ Feature — Menu Input Validation in `main.c`

**Version:** v1.4

`scanf` now has its return value checked. If the user types letters or symbols instead of a number, the invalid input is flushed from the buffer and a clear error message is shown instead of entering an infinite loop or crashing.

```c
// Before — no validation, letters caused infinite loop:
scanf("%d", &choice);

// After — validated, invalid input is flushed and reported:
if(scanf("%d", &choice) != 1)
{
    int c;
    while((c = getchar()) != '\n' && c != EOF);
    printf(H_RED "Invalid input! Please enter a number.\n" RESET);
    continue;
}
```

---

## ✨ Feature — Automated Test Target in `makefile`

**Version:** v1.4

`make test` was added to the makefile. It generates four `.txt` test files, builds an automated input sequence covering create, display, update, search, and exit, then pipes it into the program — producing a full end-to-end run without any manual interaction.

`make clean` was also updated to remove all generated test files (`test*.txt`) and `database.txt`.

---

## 🤖 Use of Claude (AI)

| Version | Task | Role of Claude |
|---|---|---|
| v1.1 | Bug identification & root cause analysis | Reviewed source, identified all 4 bugs |
| v1.1 | Before/after documentation | Generated diff-style code comparisons |
| v1.1–v1.4 | README, CHANGELOG, PROJECT_METRICS authoring | Drafted and maintained all `.md` files |
| v1.3 | Optimization analysis | Identified sorted-chain, single-pass, snprintf improvements |
| v1.4 | Segfault diagnosis | Identified `free()` on stack memory as root cause |
| v1.4 | Punctuation stripping design | Designed smart apostrophe logic |
| All | Core implementation | **Author** — all C code, logic, and design |

All logic, data structures, algorithms, and implementation are the author's original work. Claude was used strictly as a code reviewer and documentation assistant.

---

*Changelog maintained by the project author. AI-assisted review by Claude (Anthropic).*