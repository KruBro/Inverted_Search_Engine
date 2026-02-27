# 🤝 Contributing

Thank you for your interest in the Inverted Search Engine project. This is primarily a personal C learning project, but well-considered contributions are welcome.

---

## Before You Start

- Read the [README](README.md) to understand the project architecture and data structures.
- Read the [CHANGE_LOG](CHANGE_LOG.md) to understand what has already been fixed and improved.
- Check open issues before opening a new one — your bug or idea may already be tracked.

---

## What Contributions Are Welcome

| Type | Welcome? |
|---|---|
| Bug reports (with reproduction steps) | ✅ Yes |
| Bug fixes | ✅ Yes |
| Performance improvements | ✅ Yes |
| Documentation corrections | ✅ Yes |
| New features that fit the project scope | ⚠️ Discuss in an issue first |
| Major refactors or architecture changes | ⚠️ Discuss in an issue first |

---

## How to Contribute

### 1. Fork and Clone

```bash
git clone https://github.com/KruBro/Inverted_Search_Engine.git
cd Inverted_Search_Engine
```

### 2. Build the Project

```bash
make
```

### 3. Run the Automated Tests

Always make sure the test suite passes before and after your changes:

```bash
make test
```

### 4. Make Your Changes

- Keep changes focused — one fix or feature per pull request.
- Follow the existing code style: braces on new lines, consistent indentation, `/* C-style comments */`.
- If you add a new `.c` file, add a matching declaration block in `main.h`.
- Any new heap allocations (`malloc`, `strdup`) must have a corresponding `free` path.

### 5. Memory Safety

This project uses `char *` fields allocated via `strdup`. Any contribution that allocates memory **must** be verified to be leak-free. Run with AddressSanitizer before submitting:

```bash
gcc -g -fsanitize=address -o inverted_search_asan *.c
./inverted_search_asan test1.txt test2.txt
```

### 6. Open a Pull Request

- Use a clear, descriptive title.
- Reference any related issue (e.g. `Fixes #12`).
- Describe what changed and why.
- Confirm that `make test` passes.

---

## Code Style Reference

```c
/* ── Section headers use this style ── */

// Inline comments for single-line explanations

Status some_function(hash_T *arr, Flist *head)
{
    /* Opening brace on new line */
    if(condition)
    {
        /* Even single-line if/else use braces */
    }

    return SUCCESS;
}
```

---

## Reporting a Bug

Open an issue with:
1. A short description of the bug
2. Steps to reproduce it
3. What you expected to happen
4. What actually happened
5. Your OS and GCC version

---

*This project was built as a learning exercise in C. Contributions that help it grow technically are the most valuable.*
