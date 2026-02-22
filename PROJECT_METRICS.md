# 📊 PROJECT METRICS — Inverted Search Engine

> A quantified breakdown of the project: code, bugs, performance, and structure.  
> *AI-assisted review and documentation by Claude (Anthropic).*

---

## 📁 Codebase Overview

| File | Lines of Code | Responsibility |
|---|---|---|
| `main.c` | ~100 | Entry point, menu loop, user interaction |
| `main.h` | ~55 | All structs, enums, and function declarations |
| `color.h` | ~35 | ANSI terminal color/style macros |
| `create_database.c` | ~110 | Core indexing — builds the hash table from files |
| `search_database.c` | ~30 | Word lookup across the index |
| `display_database.c` | ~40 | Formatted terminal table display |
| `save_database.c` | ~40 | Export index to `database.txt` |
| `update_database.c` | ~55 | Incremental file addition |
| `validation.c` | ~50 | File validation and duplicate detection |
| `flist_utils.c` | ~55 | Linked list utilities (insert, print, free) |
| `hash_t_utils.c` | ~35 | Hash table init and full memory free |
| `makefile` | ~10 | Build system |
| **Total** | **~615** | |

---

## 🏗️ Architecture Metrics

| Metric | Value |
|---|---|
| Number of source files | 12 |
| Number of distinct data structures | 4 (`Flist`, `mNode`, `sNode`, `hash_T`) |
| Hash table size | 27 buckets (a–z + catch-all) |
| Linked list type | Singly-linked (all three: Flist, mNode chain, sNode chain) |
| Maximum word length supported | 19 characters (`char word[20]`) |
| Maximum filename length supported | 19 characters (`char file_name[20]`) |
| Status codes defined | 5 (`SUCCESS`, `FAILURE`, `LIST_EMPTY`, `DUPLICATE`, `DATA_NOT_FOUND`) |
| ANSI color macros defined | 22 |
| Menu options | 6 |

---

## 🐛 Bug Summary

| # | Bug | File | Severity | Status |
|---|---|---|---|---|
| 1 | Infinite loop — missing outer `break` after word match | `create_database.c` | 🔴 Critical | ✅ Fixed |
| 2A | Pass-by-value — `Flist *head` not propagated to caller | `update_database.c` | 🔴 Critical | ✅ Fixed |
| 2B | `free()` called on a stack-allocated VLA | `main.c` | 🔴 Critical | ✅ Fixed |
| 3 | Duplicate detection — inner loop never compared last node | `validation.c` | 🟠 High | ✅ Fixed |
| 4 | Full re-index on every update — O(all files) instead of O(new files) | `update_database.c` | 🟡 Medium | ✅ Fixed |

**Total bugs identified:** 5 (across 4 logical categories)  
**Total bugs resolved:** 5  
**Resolution rate:** 100%

---

## ⚡ Performance Impact of Bug Fixes

### Bug #1 — Infinite Loop

| Scenario | Before Fix | After Fix |
|---|---|---|
| Any word appearing 2+ times in same file | ♾️ Infinite loop / hang | ✅ Correct O(n) traversal |
| Single-occurrence words | ✅ Worked correctly | ✅ Unchanged |

### Bug #4 — Incremental Update

| Files in Database | Files Being Added | Before (re-index cost) | After (incremental cost) |
|---|---|---|---|
| 10 files | 1 new file | Re-reads all 10 files | Reads only 1 new file |
| 100 files | 5 new files | Re-reads all 100 files | Reads only 5 new files |
| N files | K new files | O(total words in N files) | O(total words in K files) |

For large databases, the incremental update reduces indexing time proportionally to `(K / N)` — a significant speedup as the database grows.

---

## 🧠 Complexity Analysis

### `create_database`

| Phase | Time Complexity |
|---|---|
| Outer loop (files) | O(F) — number of files |
| Inner loop (words per file) | O(W) — words per file |
| Collision chain traversal | O(C) — words per hash bucket |
| Sub-node traversal | O(S) — files per word |
| **Overall** | **O(F × W × C × S)** — in practice much faster due to hash distribution |

### `search_database`

| Phase | Time Complexity |
|---|---|
| Hash computation | O(1) |
| Chain traversal (case-insensitive) | O(C) — words in bucket |
| Sub-node result printing | O(S) — files the word appears in |
| **Overall** | **O(C + S)** |

### `update_database` (after fix)

| Phase | Time Complexity |
|---|---|
| Finding old tail | O(F) |
| Validating new files | O(K) |
| Indexing new files | O(K × W × C × S) |
| **Overall** | **O(F + K × W × C × S)** ← vs. O(N × W × C × S) before |

---

## 💾 Memory Usage

| Structure | Memory per Node |
|---|---|
| `Flist` node | `sizeof(char[20]) + sizeof(ptr)` ≈ 28 bytes |
| `mNode` (main node) | `sizeof(u_int) + sizeof(char[20]) + 2×sizeof(ptr)` ≈ 36 bytes |
| `sNode` (sub node) | `sizeof(u_int) + sizeof(char[20]) + sizeof(ptr)` ≈ 28 bytes |
| `hash_T` entry | `sizeof(u_int) + sizeof(ptr)` ≈ 12 bytes |
| Full hash table (stack) | `27 × sizeof(hash_T)` ≈ 324 bytes |

All `mNode` and `sNode` allocations are heap-allocated via `malloc`. The hash table itself lives on the stack in `main()`. Memory is fully freed before exit (verified via `free_hash_table` + `free_list`).

---

## 🤖 AI Assistance Log

| Task | Tool Used | Notes |
|---|---|---|
| Bug review & root cause analysis | Claude (Anthropic) | All 4 bug categories identified |
| `CHANGELOG.md` authoring | Claude (Anthropic) | Before/after code diffs generated |
| `README.md` authoring | Claude (Anthropic) | Architecture diagram, tables, docs |
| `PROJECT_METRICS.md` authoring | Claude (Anthropic) | This file |
| Complexity analysis | Claude (Anthropic) | Big-O breakdown of key functions |
| Core implementation | **Author** | All C code, logic, and design |

> All source code, data structures, and algorithms are the original work of the project author. Claude was used exclusively as a code review and documentation assistant.

---

*Generated with AI-assisted documentation. Core project by the author.*