# Simple Shell Interpreter

A minimal Unix shell implemented in C, written as a first-year systems programming exercise. Supports command execution, foreground processes, and the `cd` built-in.

---

## Features

- Executes standard Unix commands via `execvp`
- Foreground process execution with `fork` / `waitpid`
- Built-in `cd` command
- Reads and parses user input line by line

---

## Build & Run

```bash
make        # compiles to executable 'ssi'
./ssi       # start the shell
```

---

## Project Structure

```
Simple-shell-Interpreter/
├── main.c      # Shell logic
└── Makefile
```

---

*First-year systems programming project — University of Victoria, 2022.*
