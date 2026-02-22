# 🔍 Inverted Search Engine — C Project

> A terminal-based **Inverted Index Search Engine** written in C, using a custom hash table with chained linked lists to index words across multiple `.txt` files.

---

## 📌 Table of Contents

- [Overview](#overview)
- [Architecture](#architecture)
- [Data Structures](#data-structures)
- [Project Structure](#project-structure)
- [Features](#features)
- [How to Build & Run](#how-to-build--run)
- [Menu Options](#menu-options)
- [Known Limitations](#known-limitations)
- [Changelog & Bug Fixes](#changelog--bug-fixes)

---

## Overview

This project implements an **Inverted Index** — a classic data structure used in search engines — entirely in C. Given one or more `.txt` files, the program indexes every word, recording:

- Which files each word appears in
- How many times it appears in each file

The user can then query any word and instantly see all files it was found in, along with occurrence counts.

---

## Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                        Hash Table (size 27)                         │
│   [a] → [b] → ... → [z] → [#]  (index 26 = non-alphabetic words)   │
└────────────────────────┬────────────────────────────────────────────┘
                         │
                    ┌────▼─────┐
                    │  mNode   │  ← Main Node (one per unique word)
                    │  word[]  │
                    │filecount │
                    └──┬────┬──┘
                       │    └─────────────────────────────┐
                  mLink▼                             sLink▼
              ┌──────────┐                       ┌──────────┐
              │  mNode   │                       │  sNode   │  ← Sub Node
              │  "word2" │                       │ file.txt │
              └──────────┘                       │  count   │
                                                 └────┬─────┘
                                                 subLink▼
                                                 ┌──────────┐
                                                 │  sNode   │
                                                 │ file2.txt│
                                                 └──────────┘
```

The hash table has **27 buckets** — one per letter of the alphabet (index by first character, case-insensitive), and one catch-all bucket (index 26) for tokens beginning with non-alphabetic characters.

---

## Data Structures

### `Flist` — File List
A singly-linked list tracking all files that have been loaded into the system.

```c
typedef struct Node {
    char file_name[20];
    struct Node *link;
} Flist;
```

### `mNode` — Main Node
One node per unique word. Stores the word itself, how many files it appears in, a pointer to the first sub-node, and a pointer to the next word in the same hash bucket.

```c
typedef struct mainNode {
    u_int filecount;
    char  word[20];
    sNode *sLink;
    struct mainNode *mLink;
} mNode;
```

### `sNode` — Sub Node
One node per file a word appears in. Stores the filename and the count of occurrences.

```c
typedef struct subNode {
    u_int wordcount;
    char  file_name[20];
    struct subNode *subLink;
} sNode;
```

### `hash_T` — Hash Table Entry
Array of 27 of these, each holding an index and a pointer to the head of its `mNode` chain.

```c
typedef struct hashT {
    u_int  index;
    mNode *link;
} hash_T;
```

---

## Project Structure

```
inverted_search/
├── main.c                  # Entry point, menu loop, user interaction
├── main.h                  # All structs, enums, and function declarations
├── color.h                 # ANSI color/style macros for terminal output
├── create_database.c       # Core indexing logic — reads files, builds the hash table
├── search_database.c       # Word lookup across the hash table
├── display_database.c      # Pretty-prints the entire database as a colored table
├── save_database.c         # Saves the database to database.txt
├── update_database.c       # Adds new files to an existing database (incremental)
├── validation.c            # File validation (extension, existence, empty, duplicate)
├── flist_utils.c           # Flist insert, print, free utilities
├── hash_t_utils.c          # Hash table init and free utilities
└── makefile                # Build system
```

---

## Features

| Feature | Description |
|---|---|
| **Multi-file indexing** | Pass any number of `.txt` files as arguments |
| **Case-insensitive search** | `Hello` and `hello` are treated as the same word |
| **Duplicate file detection** | The same file cannot be indexed twice |
| **Incremental update** | Add new files without re-indexing existing ones |
| **Colorized terminal output** | Full ANSI color support via `color.h` |
| **Save to file** | Export the full index to `database.txt` |
| **Non-alphabetic word support** | Tokens starting with digits or symbols go into bucket 26 |

---

## How to Build & Run

### Requirements
- GCC (any modern version)
- A POSIX-compatible terminal (Linux/macOS recommended)

### Build
```bash
make
```

### Run
```bash
./inverted_search.exe file1.txt file2.txt file3.txt
```

### Clean
```bash
make clean
```

---

## Menu Options

```
1. Create Database    — Index all loaded files into the hash table
2. Display Database   — Print the full index as a formatted, colored table
3. Search Database    — Look up a specific word
4. Update Database    — Add new .txt files to the existing index
5. Save Database      — Write the index to database.txt
6. Exit               — Save, free all memory, and quit cleanly
```

---

## Known Limitations

- Word buffer is fixed at `char word[20]` — words longer than 19 characters will be silently truncated.
- File name buffer is fixed at `char file_name[20]` — filenames longer than 19 characters will cause a buffer overflow.
- The `compare()` function in `validation.c` contains a structural bug (see [CHANGELOG.md](CHANGELOG.md)) that was identified and documented; the duplicate detection is handled correctly via `insert_at_last`.

---

## Changelog & Bug Fixes

See [CHANGELOG.md](CHANGELOG.md) for a full record of every bug identified, its root cause, and how it was resolved.

See [PROJECT_METRICS.md](PROJECT_METRICS.md) for a quantified breakdown of the project.

---

*Built entirely in C. Terminal colors powered by `color.h` using ANSI escape codes.*