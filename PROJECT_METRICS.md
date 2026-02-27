# 📊 PROJECT METRICS — Inverted Search Engine

> A quantified breakdown of the project: code, bugs, performance, and structure.  
> *AI-assisted review and documentation by Claude (Anthropic).*

---

## 📁 Codebase Overview

| File | Lines of Code | Responsibility |
|---|---|---|
| `main.c` | ~110 | Entry point, menu loop, input validation |
| `main.h` | ~80 | All structs, enums, and function declarations |
| `color.h` | ~35 | ANSI terminal color/style macros |
| `create_database.c` | ~120 | Core indexing — builds the hash table from files |
| `search_database.c` | ~45 | Prefix-aware word lookup across the index |
| `display_database.c` | ~45 | Formatted terminal table display |
| `save_database.c` | ~45 | Export index to `database.txt` |
| `update_database.c` | ~55 | Incremental file addition |
| `validation.c` | ~50 | File validation and duplicate detection |
| `flist_utils.c` | ~70 | Linked list utilities (insert, print, free) |
| `hash_t_utils.c` | ~50 | Hash table init and full memory free |
| `files_utils.c` | ~25 | `strip_punctuation` — token cleaning |
| `makefile` | ~30 | Build system with automated test target |
| **Total** | **~760** | |

---

## 🏗️ Architecture Metrics

| Metric | Value |
|---|---|
| Number of source files | 13 |
| Number of distinct data structures | 4 (`Flist`, `mNode`, `sNode`, `hash_T`) |
| Hash table size | 27 buckets (a–z + catch-all) |
| Linked list type | Singly-linked (all three: Flist, mNode chain, sNode chain) |
| String storage | Heap-allocated via `strdup` (no fixed-size limits) |
| Status codes defined | 5 (`SUCCESS`, `FAILURE`, `LIST_EMPTY`, `DUPLICATE`, `DATA_NOT_FOUND`) |
| ANSI color macros defined | 22 |
| Menu options | 6 |

---

## 🐛 Bug Summary

| # | Bug | File | Severity | Version Fixed | Status |
|---|---|---|---|---|---|
| 1 | Infinite loop — missing outer `break` after word match | `create_database.c` | 🔴 Critical | v1.1 | ✅ Fixed |
| 2A | Pass-by-value — `Flist *head` not propagated to caller | `update_database.c` | 🔴 Critical | v1.1 | ✅ Fixed |
| 2B | `free()` called on a stack-allocated VLA | `main.c` | 🔴 Critical | v1.1 | ✅ Fixed |
| 3 | Duplicate detection — inner loop compared wrong pointer | `validation.c` | 🟠 High | v1.1 | ✅ Fixed |
| 4 | Full re-index on every update — O(N) instead of O(K) | `update_database.c` | 🟡 Medium | v1.2 | ✅ Fixed |
| 5 | Segfault on exit — `free()` called on non-heap string arrays | `hash_t_utils.c` / `main.h` | 🔴 Critical | v1.4 | ✅ Fixed |

**Total bugs identified:** 6 (across 5 logical categories)  
**Total bugs resolved:** 6  
**Resolution rate:** 100%

---

## ✨ Feature & Optimization Summary

| # | Change | Files Affected | Version |
|---|---|---|---|
| O1 | Sorted mNode chains — early-exit traversal | `create_database.c` | v1.3 |
| O2 | Single-pass `insert_at_last` — duplicate check + tail find combined | `flist_utils.c` | v1.3 |
| O3 | `snprintf` with running offset — O(n) filename concatenation | `display_database.c`, `save_database.c` | v1.3 |
| F1 | `strip_punctuation` — punctuation and smart apostrophe stripping | `files_utils.c`, `create_database.c` | v1.4 |
| F2 | Prefix search — `strncasecmp` full-chain scan | `search_database.c` | v1.4 |
| F3 | Menu input validation — non-numeric input handled gracefully | `main.c` | v1.4 |
| F4 | `make test` — automated end-to-end test flow | `makefile` | v1.4 |
| F5 | Dynamic string allocation — `char *` + `strdup` in all structs | `main.h`, all `.c` files | v1.4 |

---

## ⚡ Performance Impact

### Bug #1 — Infinite Loop

| Scenario | Before | After |
|---|---|---|
| Word appearing 2+ times in same file | ♾️ Infinite hang | ✅ Correct O(n) traversal |
| Single-occurrence words | ✅ Worked correctly | ✅ Unchanged |

### Bug #4 / Optimization O1 — Update & Chain Traversal

| Files in DB | Files Added | Before (re-index cost) | After (incremental cost) |
|---|---|---|---|
| 10 files | 1 new file | Re-reads all 10 | Reads only 1 |
| 100 files | 5 new files | Re-reads all 100 | Reads only 5 |
| N files | K new files | O(total words in N) | O(words in K) |

| Chain search | Before (unsorted) | After (sorted) |
|---|---|---|
| Hit (word found) | O(C) — full scan to match | O(C/2) avg — stops at match |
| Miss (word absent) | O(C) — full scan to NULL | O(C/2) avg — early exit when passed |

---

## 🧠 Complexity Analysis

### `create_database`

| Phase | Time Complexity |
|---|---|
| Outer loop (files) | O(F) |
| Inner loop (words per file) | O(W) |
| Chain traversal (sorted, early-exit) | O(C/2) average |
| Sub-node traversal | O(S) |
| **Overall** | **O(F × W × C/2 × S)** |

### `search_database` (prefix)

| Phase | Time Complexity |
|---|---|
| Hash computation | O(1) |
| Full chain scan for prefix matches | O(C) — must scan all, not just until first match |
| Sub-node result printing per match | O(S) per match |
| **Overall** | **O(C + M × S)** where M = number of prefix matches |

### `update_database`

| Phase | Time Complexity |
|---|---|
| Finding old tail (no tail pointer) | O(F) |
| Validating K new files | O(K) |
| Indexing K new files | O(K × W × C/2 × S) |
| **Overall** | **O(F + K × W × C/2 × S)** |

---

## 💾 Memory Usage

All string fields are now heap-allocated via `strdup` — struct sizes reflect pointer widths rather than fixed char arrays.

| Structure | Memory per Node |
|---|---|
| `Flist` node | `sizeof(ptr) + sizeof(ptr)` ≈ 16 bytes + heap string |
| `mNode` | `sizeof(u_int) + sizeof(ptr) + 2×sizeof(ptr)` ≈ 28 bytes + heap string |
| `sNode` | `sizeof(u_int) + sizeof(ptr) + sizeof(ptr)` ≈ 20 bytes + heap string |
| `hash_T` entry | `sizeof(u_int) + sizeof(ptr)` ≈ 12 bytes |
| Full hash table (stack) | `27 × sizeof(hash_T)` ≈ 324 bytes |

Heap strings (`strdup`) are allocated and freed alongside their nodes. All memory is fully released before exit — verified via AddressSanitizer (`-fsanitize=address,leak`).

---

## 🤖 AI Assistance Log

| Version | Task | Tool | Notes |
|---|---|---|---|
| v1.1 | Bug review & root cause analysis | Claude (Anthropic) | All 5 bugs identified |
| v1.1 | Before/after code diff documentation | Claude (Anthropic) | CHANGELOG diffs generated |
| v1.1 | README, CHANGELOG, PROJECT_METRICS authoring | Claude (Anthropic) | All `.md` files drafted |
| v1.3 | Optimization identification & implementation | Claude (Anthropic) | Sorted chains, single-pass, snprintf |
| v1.4 | Segfault root cause diagnosis | Claude (Anthropic) | Identified free-on-stack-array issue |
| v1.4 | `strip_punctuation` design | Claude (Anthropic) | Smart apostrophe logic designed |
| v1.4 | `.md` updates for all new changes | Claude (Anthropic) | This file |
| All | Core implementation | **Author** | All C code, logic, and design |

> All source code, data structures, and algorithms are the original work of the project author. Claude was used exclusively as a code reviewer and documentation assistant.

---

*Generated with AI-assisted documentation. Core project by the author.*