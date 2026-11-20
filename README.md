# shaheerLang

**A small programming language built from scratch in C++.**

`shaheerLang` is a personal project where I explored the fundamentals of programming language design by building a simple interpreted language of my own.

The goal was to understand what happens behind the scenes when code is written and executed — from lexical analysis and parsing to interpreting expressions and managing variables.

Rather than relying on an existing language framework, I implemented the core components myself in C++.

## Overview

The interpreter follows a traditional execution pipeline:

```text
Source Code (.net)
       |
       v
     Lexer
       |
       v
    Tokens
       |
       v
     Parser
       |
       v
   Statements
       |
       v
   Interpreter
       |
       v
     Output
```

Each component has its own responsibility, keeping the implementation separated and easier to understand.

## Features

The current implementation includes:

- **Lexical Analysis** – Tokenizes source code into meaningful units.
- **Parsing** – Processes tokens and builds expressions and statements.
- **Variable Declaration** – Supports declaring variables using `define`.
- **Data Types** – Basic integer, decimal, and string values.
- **Expressions** – Arithmetic and comparison operations.
- **Variable Assignment** – Updates existing variable values.
- **Console Output** – Prints values using `logOnConsole()`.

This is an early-stage implementation, so the available language features are still limited.

## Getting Started

### Prerequisites

You'll need:

- A C++ compiler with C++14 support or later.
- Git.

### Clone the Repository

```bash
git clone https://github.com/shaheer-ali-dev/shaheerLang.git
cd shaheerLang
```

### Build

Using CMake (recommended):

```bash
cmake -S . -B build
cmake --build build
```

Or directly with GCC:

```bash
g++ -std=c++14 -Iinclude src/main.cpp src/lexer/*.cpp src/parser/*.cpp \
    src/interpreter/*.cpp src/runtime/*.cpp -o shaheerLang
```

### Run

```bash
./build/shaheerLang examples/hello.net    # run a specific file
./build/shaheerLang                       # defaults to examples/hello.net
```

On Windows, use `.\build\Debug\shaheerLang.exe` (or `.\shaheerLang.exe` if built with GCC).

### Test

```bash
ctest --test-dir build --output-on-failure
```

## Writing Your First Program

The repository includes a sample program in `examples/hello.net`.

```javascript
define x = "SHAHEER";
define y = 12.5;

logOnConsole(x);
```

The interpreter reads the file, tokenizes the source, parses the resulting tokens, and executes the statements.

Expected output:

```text
SHAHEER
```

You can modify `examples/hello.net` (or pass your own file) to experiment with the language's currently implemented syntax.

## Project Structure

```text
shaheerLang/
│
├── include/
│   └── shaheer/
│       ├── Lexer.h
│       ├── Parser.h
│       ├── Interpreter.h
│       ├── Token.h
│       ├── Expr.h
│       ├── Stmt.h
│       ├── Environment.h
│       └── Value.h
│
├── src/
│   ├── lexer/
│   │   └── Lexer.cpp
│   ├── parser/
│   │   └── Parser.cpp
│   ├── interpreter/
│   │   └── Interpreter.cpp
│   ├── runtime/
│   │   ├── Environment.cpp
│   │   └── Value.cpp
│   └── main.cpp
│
├── tests/
│   ├── lexer_tests.cpp
│   ├── parser_tests.cpp
│   └── interpreter_tests.cpp
│
├── examples/
│   └── hello.net
│
├── CMakeLists.txt
├── README.md
└── .gitignore
```

### Core Components

| Component | Responsibility |
|---|---|
| Lexer | Converts source code into tokens |
| Parser | Processes tokens and constructs expressions and statements |
| Interpreter | Evaluates expressions and executes statements |
| Environment | Stores and manages variables |
| Expr | Defines expression structures |
| Stmt | Defines statement structures |
| Token | Defines token types and token data |
| Value | Represents runtime values |

## Development Status

This project started as a learning exercise and an opportunity to explore how programming languages work internally.

Development has slowed down due to other commitments, so the project is currently on hold. However, I still intend to return to it and continue building on what I've started.

There is plenty left to explore, and this repository represents an early stage of that process.

## Why I Built This

I wanted to move beyond simply learning how to use programming languages and understand how they actually work.

Building a lexer, parser, and interpreter helped me explore concepts that are usually hidden behind compilers and language runtimes.

It's a small project, but one that gave me a much better appreciation of what's happening underneath the code we write every day.

---

**Author:** [Shaheer Ali](https://github.com/shaheer-ali-dev)

**Language:** C++  
**Project Type:** Programming Language / Interpreter  
**Status:** Work in Progress
