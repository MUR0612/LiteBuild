# LiteBuild

LiteBuild is a lightweight build automation tool written in C. It is inspired by the UNIX `make` workflow and implements a small build engine that can parse a custom build file, resolve file dependencies, compare modification times, expand variables, and execute commands through POSIX process APIs.

This project demonstrates systems programming skills in C, including lexical parsing, dynamic data structures, recursive dependency resolution, process creation, pipeline execution, signal handling, and manual memory management.

## Features

- Parses a custom build file format from the default `Litefile` or a file provided with `--file`
- Supports variable definitions and command-line variable overrides
- Resolves requested targets and recursively builds their dependencies
- Rebuilds file targets only when dependencies are newer than the target
- Supports implicit `.o` builds from matching `.c` source files
- Supports phony targets and multi-stage command pipelines with `=>`
- Provides `--simulate` mode to print build commands without executing them
- Tracks active child processes and handles `SIGINT` shutdown safely

## Tech Stack

- C
- GCC
- POSIX APIs
- Makefile
- Linux / Ubuntu / WSL

Key APIs and concepts used:

- `fork()`, `execvp()`, `waitpid()`
- `pipe()`, `dup2()`
- `stat()` and file modification timestamps
- `sigprocmask()`, `sigtimedwait()`, `kill()`
- Dynamic arrays and hash maps
- Manual heap allocation and cleanup

## Project Structure

```text
LiteBuild/
├── Makefile
├── main.c
├── lexer.c
├── lexer.h
├── core/
│   ├── types.h
│   └── variable.c
├── frontend/
│   ├── cli.c
│   └── parser.c
├── engine/
│   ├── build.c
│   └── exec.c
└── utils/
    ├── hashmap.c
    ├── logger.c
    ├── memory.c
    └── vector.c

Build
make

This produces the executable:

./litebuild

Clean build outputs:

make clean
Usage

Run LiteBuild using the default Litefile:

./litebuild

Run a specific target:

./litebuild app

Use a custom build file:

./litebuild --file Litefile app

Print commands without executing them:

./litebuild --simulate app

Override variables from the command line:

./litebuild COMPILER=gcc app
Litefile Syntax

Variable definition:

COMPILER = gcc
COMPILEFLAGS = -Wall -Wextra -pedantic

File target with dependencies:

app : main.o parser.o

Phony target with a command:

clean ~ rm -f app main.o parser.o

Phony target with a multi-stage command pipeline:

check ~ cat results.txt => grep PASS => wc -l
Example Litefile
COMPILER = gcc
COMPILEFLAGS = -Wall -Wextra -pedantic

app : main.o parser.o

clean ~ rm -f app main.o parser.o

check ~ cat results.txt => grep PASS => wc -l

Example run:

./litebuild app
./litebuild --simulate clean
./litebuild check
Design Overview

LiteBuild is organized into four main layers:

frontend/: command-line parsing and Litefile parsing
core/: shared data models for variables, targets, and runtime state
engine/: dependency resolution, rebuild decisions, command execution, and signal handling
utils/: reusable memory, logging, vector, and hash map utilities

The build flow is:

Parse command-line options.
Read and tokenize the Litefile.
Build an internal model of variables and targets.
Apply command-line variable overrides.
Resolve requested targets or use the first target by default.
Recursively build dependencies.
Compare file timestamps to decide whether rebuilds are needed.
Execute commands through POSIX child processes.
Dependency Resolution

LiteBuild recursively evaluates file targets and their dependencies. For file targets, it uses stat() to inspect modification timestamps and only rebuilds when:

the target file does not exist, or
at least one dependency was rebuilt, or
at least one dependency is newer than the target.

For .o targets, LiteBuild can use an implicit rule by looking for a matching .c source file and generating a compile command.

Process Execution

LiteBuild executes build commands using low-level POSIX process control:

fork() creates child processes.
execvp() replaces child processes with requested commands.
waitpid() reaps completed children.
pipe() and dup2() connect multi-stage command pipelines.

For phony pipeline targets, commands separated by => are split into pipeline stages and connected through UNIX pipes.

Signal Handling

LiteBuild tracks running child processes in a runtime state structure. When SIGINT is received, it marks the runtime as interrupted, sends termination signals to active children, and prevents child processes from being left behind.

This part of the project focuses on safe process lifecycle management and graceful shutdown behavior.

Memory Management

The project uses manual memory management throughout the parser, model, variable table, vectors, and hash map. Utility wrappers such as xmalloc(), xrealloc(), and xstrdup() centralize allocation failure handling.

Major heap-owned structures include:

token arrays produced by the lexer
variable definitions and expanded variables
target definitions and dependency vectors
hash map entries
runtime child PID arrays

Each major model has a matching cleanup path to release owned memory before program exit.

Validation

Suggested local checks:

make clean
make
./litebuild --simulate
valgrind --leak-check=full ./litebuild --simulate

Suggested compiler flags:

-Wall -Wextra -pedantic -std=gnu99 -g -O2
Portfolio Focus

This project demonstrates:

C systems programming
Linux process management
File dependency tracking
Parser and lexer implementation
Dynamic data structure design
Hash map based lookup
Build automation workflow
Signal-aware command execution
Manual memory management
Future Improvements
Add automated regression tests with sample Litefiles
Add cycle detection for dependency graphs
Add benchmark cases for target lookup and dependency traversal
Add more detailed error messages for invalid Litefile syntax
Add support for richer build file syntax