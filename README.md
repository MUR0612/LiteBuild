# LiteBuild

A custom, lightweight, and POSIX-compliant build automation tool written entirely in C. Inspired by the UNIX `make` utility, LiteBuild parses custom makefiles, resolves complex dependency graphs, and executes build processes via multi-stage pipelines.

## Key Features

* **Custom Lexer & Parser**: Implemented a standalone lexical analyzer from scratch to parse build rules, variables, and dependencies without relying on external regex or parsing libraries.
* **O(1) Hash Map Optimization**: Replaced linear array searches with a custom-built Hash Map for target and variable lookups, significantly improving performance for large-scale dependency graphs.
* **Smart Dependency Resolution**: Recursively evaluates target dependencies and utilizes `stat()` to compare file modification timestamps (nanosecond precision), ensuring targets are only rebuilt when absolutely necessary.
* **Multi-Stage Command Pipelines**: Supports inter-process communication (IPC) via `pipe()` and `dup2()`. Commands linked with `=>` are executed in parallel pipelines with seamless I/O redirection.
* **Robust Process & Signal Management**:
  * Utilizes low-level POSIX APIs (`fork`, `execvp`, `waitpid`) to spawn and reap child processes dynamically.
  * Implements safe asynchronous signal handling (`sigtimedwait`, `kill`). Intercepting `SIGINT` (Ctrl-C) triggers a graceful shutdown mechanism that broadcasts termination signals to active child processes and prevents zombie processes.

## Architecture

The project is heavily modularized for maintainability and scalability:
* `frontend/`: Command-line interface parsing and lexical/syntactical analysis.
* `engine/`: The core build engine, dependency graph traversal, and process execution lifecycle.
* `core/`: Core definitions of Targets, Variables, and the BuildFile internal model.
* `utils/`: Memory-safe dynamic data structures (`Vector`, `HashMap`) and standardized logging.

## Usage

**Build the tool:**
```bash
make
