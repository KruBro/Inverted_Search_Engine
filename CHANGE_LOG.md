# 📋 CHANGELOG — Inverted Search Engine

> A full record of every bug identified, its root cause, the original problematic code, and the fix applied. Claude (AI) was used as a reviewing and documentation assistant during this phase of the project.

---

## Version History

| Version | Description |
|---|---|
| `v1.0` | Initial implementation — core data structures, hash table, and menu loop |
| `v1.1` | Bug fixes across four critical areas (documented below) |
| `v1.2` | Incremental update architecture introduced |

---

## 🐛 Bug #1 — Infinite Loop in `create_database.c`

**File:** `create_database.c`  
**Severity:** Critical — causes the program to hang on any word that appears more than once in the same file.

### Root Cause

When a word was already present in the hash table **and** a sub-node for the current file already existed, the code correctly incremented `sTemp->wordcount` and called `break` — but this only broke the **inner** `while(sTemp)` loop. The **outer** `while(mTemp)` loop had no corresponding `break`, so after finding the word and updating its count, the loop would continue iterating over other main nodes indefinitely (or until `mTemp` became NULL by accident).

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
                break;          // ✅ breaks inner loop correctly
                                // ❌ outer loop still continues!
            }
            // ...
        }
        // ← No break here. Outer loop keeps running.
    }
    else
    {
        mPrev = mTemp;
        mTemp = mTemp->mLink;
    }
}
```

### After (Fixed)

The outer `if` block that handles a found word now contains its own `break` at the end, ensuring the outer loop exits after a match is found and processed:

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
                break;
            }
            else
            {
                sPrev = sTemp;
                sTemp = sTemp->subLink;
            }
        }

        if(sTemp == NULL) // word is in a new file
        {
            sNode *new_subNode = malloc(sizeof(sNode));
            // ... setup new_subNode ...
            sPrev->subLink = new_subNode;
            (mTemp->filecount)++;
        }

        break;  // ✅ Now correctly exits the outer loop too
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

**File:** `update_database.c`  
**Severity:** Critical — causes silent data loss and undefined behavior (crash).

### Root Cause (Part A) — Pass by Value

The original `update_database` function received `Flist *head` (a copy of the pointer), not `Flist **head`. Any new nodes added to the linked list inside the function were lost the moment the function returned, because the caller's `head` pointer was never updated.

### Root Cause (Part B) — Freeing a Stack Array

After collecting file names from the user into a VLA (`char *fileHolder[fileCount]`), the original code attempted to call `free(fileHolder)`. Since `fileHolder` is stack-allocated, calling `free()` on it is undefined behavior and would typically cause an immediate crash or heap corruption.

### Before (Broken)

```c
// In main.c (caller):
update_database(hash_t, head, fileHolder, fileCount);
//                      ^^^^ — passing by value, changes are lost

// In update_database.c (old signature):
Status update_database(hash_T *arr, Flist *head, char *fileHolder[], u_int fileCount)
//                                  ^^^^^^^^^^^ — copy, not pointer-to-pointer

// After the function (old main.c):
for(int i = 0; i < fileCount; i++)
    free(fileHolder[i]);  // ✅ correct — these were strdup'd
free(fileHolder);         // ❌ CRASH — fileHolder is a stack VLA
```

### After (Fixed)

The function signature was updated to accept `Flist **head`, and the stack array is no longer `free()`'d (only the individual `strdup`'d strings are freed):

```c
// Correct signature:
Status update_database(hash_T *arr, Flist **head, char **fileHolder, u_int fileCount)

// Caller passes address of head:
update_database(hash_t, &head, fileHolder, fileCount);

// Only free the strdup'd strings, not the stack array:
for(int i = 0; i < fileCount; i++)
    free(fileHolder[i]);
// No free(fileHolder) — it lives on the stack
```

---

## 🐛 Bug #3 — Duplicate File Detection Logic in `validation.c`

**File:** `validation.c`  
**Severity:** High — allows the same file to be indexed multiple times, inflating word counts.

### Root Cause

The `compare()` function was supposed to detect if a filename was already in the list. However, the comparison logic was placed inside a **nested** loop where `temp1` (the inner pointer) was advanced but never actually used in the comparison — the check `strcmp(temp->file_name, fname)` only compared against the **outer** `temp` pointer. This meant that if the duplicate was the very last node in the list, it would never be caught.

Additionally, `compare()` was architecturally redundant since `insert_at_last()` already calls `compare()` before inserting. The logic was consolidated so that duplicate detection is cleanly handled in one place.

### Before (Broken Logic in `compare()`)

```c
Status compare(Flist *head, char *fname)
{
    Flist *temp = head;
    while(temp)
    {
        Flist *temp1 = temp->link;  // temp1 is declared...
        while(temp1)
        {
            if(strcmp(temp->file_name, fname) == 0)  // ❌ uses temp, not temp1
                return DUPLICATE;
            temp1 = temp1->link;  // ...but the comparison never uses it
        }
        temp = temp->link;
    }
    return SUCCESS;
}
```

Because the inner loop compares `temp->file_name` (not `temp1->file_name`), the last node in the list is checked zero times — the inner `while(temp1)` loop exits immediately when `temp1 = temp->link = NULL` (last node), so the comparison is never reached for the last element.

### After (Fixed)

The `compare()` function is corrected to iterate over all nodes and compare each against `fname`:

```c
Status compare(Flist *head, char *fname)
{
    if(head == NULL)
        return LIST_EMPTY;

    Flist *temp = head;
    while(temp)
    {
        if(strcmp(temp->file_name, fname) == 0)
            return DUPLICATE;
        temp = temp->link;
    }
    return SUCCESS;
}
```

---

## 🐛 Bug #4 — Inefficient Full Re-index in `update_database.c`

**File:** `update_database.c`  
**Severity:** Medium — major performance regression on large datasets; functionally incorrect when the hash table is cleared and rebuilt.

### Root Cause

The original update mechanism worked by:
1. Clearing the entire hash table (`free_hash_table` + `initialize_hashTable`)
2. Adding new files to the `Flist`
3. Calling `create_database` on **all files** in the list from the beginning

This meant that every update re-processed all previously indexed files. On a large database, this compounds linearly. It also introduced the risk that if `read_and_validation` failed for a previously valid file (e.g., file moved or deleted), data would be permanently lost.

### Before (Inefficient)

```c
// Old update_database pseudocode:
free_hash_table(arr);           // wipe everything
initialize_hashTable(arr);      // start from scratch
// add new files to list
create_database(arr, *head);    // re-index ALL files
```

### After (Incremental Update)

The new implementation:
1. Records a pointer to the **last existing node** in the list before adding new files
2. Adds new files to the list
3. Calls `create_database` starting from **the node after the old tail** — only new files are processed

```c
// Save the current tail before adding anything
Flist *lastNode = *head;
if (lastNode != NULL)
    while (lastNode->link != NULL)
        lastNode = lastNode->link;

// Add new files to the list
// ...

// Index only from the new entries onward
Flist *startNode = (lastNode == NULL) ? *head : lastNode->link;
if (startNode != NULL)
    create_database(arr, startNode);
```

This reduces re-indexing time from O(total words across all files) to O(words in new files only).

---

## 🤖 Use of Claude (AI)

Claude (Anthropic) was used in the following capacities during `v1.1` and `v1.2` development:

| Task | Role of Claude |
|---|---|
| Bug identification | Reviewed source code and identified all four bugs above |
| Root cause analysis | Explained why each bug occurred at the C language level |
| Before/after documentation | Generated the diff-style code comparisons in this changelog |
| README generation | Drafted the `README.md` and `PROJECT_METRICS.md` files |
| Comment authoring | Suggested inline documentation style for source files |

All logic, data structures, algorithms, and implementation are the author's original work. Claude was used strictly as a code reviewer and documentation assistant.

---

*Changelog maintained by the project author. AI-assisted review by Claude (Anthropic).*