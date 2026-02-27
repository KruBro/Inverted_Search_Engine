# 🔒 Security Policy

## Supported Versions

This is a personal learning project. Only the latest version on the `main` branch is actively maintained.

| Version | Supported |
|---|---|
| Latest (`main`) | ✅ Yes |
| Older commits | ❌ No |

---

## Known Security Considerations

This project is a **terminal-based educational tool** intended to be run locally with trusted input files. It is not designed for networked, multi-user, or production environments. The following are known constraints:

- **No input sanitisation beyond punctuation stripping** — `fscanf` reads words up to 1024 characters; files with extremely long tokens could overflow the local buffer.
- **No path traversal protection** — filenames are passed directly to `fopen`. Do not run this program with untrusted filenames from external sources.
- **No file permission checks** — any readable `.txt` file on the system can be indexed.

---

## Reporting a Vulnerability

If you find a security issue (e.g. a buffer overflow, use-after-free, or memory corruption bug):

1. **Do not open a public issue.**
2. Contact the maintainer directly via GitHub (open a private security advisory if available, or send a direct message).
3. Include a clear description of the vulnerability and steps to reproduce it.

Reports will be acknowledged within 7 days. Given the scope of this project, fixes will be applied on a best-effort basis.

---

*This project is for educational purposes. It is not intended for deployment in security-sensitive contexts.*
