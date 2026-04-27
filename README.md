# PL/0 Compiler

A complete compiler implementation for the PL/0 programming language, written in C.

## Overview

This is a **PL/0 compiler** that translates PL/0 source code into executable output. PL/0 is a simple imperative programming language often used in educational contexts to teach compiler design and implementation.

The compiler consists of the following main components:

- **Lexer** (`lexer.c`/`lexer.h`) - Tokenizes input source code
- **Parser** (`parser.c`/`parser.h`) - Parses tokens into an abstract syntax tree (AST)
- **AST** (`ast.c`/`ast.h`) - Manages abstract syntax tree and symbol table
- **Code Generator** (`code_generator.c`/`code_generator.h`) - Generates code from the AST

## Building

### Requirements
- GCC compiler (or compatible C compiler)
- GNU Make
- MSYS2 (for Windows) or standard Unix/Linux environment

### Compilation
```bash
make
```

This will compile all source files and produce the `pl0c` executable.

### Clean Build
```bash
make clean
```

Removes all object files and executables, then rebuild.

## Usage

```bash
./pl0c <input-file.pl0>
```

Example:
```bash
./pl0c program.pl0
```

## Language Grammar

```
program     = block "." .
block       = [ "const" ident "=" number { "," ident "=" number } ";" ]
              [ "var" ident { "," ident } ";" ]
              { "procedure" ident ";" block ";" } statement .
statement   = [ ident ":=" expression
              | "call" ident
              | "begin" statement { ";" statement } "end"
              | "if" condition "then" statement
              | "while" condition "do" statement ] .
condition   = "odd" expression
            | expression ( "=" | "#" | "<" | ">" ) expression .
expression  = [ "+" | "-" ] term { ( "+" | "-" ) term } .
term        = factor { ( "*" | "/" ) factor } .
factor      = ident
            | number
            | "(" expression ")" .
```

## Language Features

- **Constants**: `const` declarations
- **Variables**: `var` declarations with array support
- **Procedures**: Modular code with procedure definitions
- **Control Flow**: `if-then`, `while-do`, `begin-end` blocks
- **Expressions**: Arithmetic with proper operator precedence
- **Operators**: `+`, `-`, `*`, `/`, `=`, `#`, `<`, `>`
- **Comments**: Multi-line `{ ... }` and inline comments

## Project Structure

```
.
├── main.c              - Entry point and file I/O
├── lexer.c/h           - Lexical analysis
├── parser.c/h          - Syntax analysis
├── ast.c/h             - AST and symbol table management
├── code_generator.c/h  - Code generation
├── token_type.h        - Token definitions
├── Makefile            - Build configuration
├── tests/              - Test programs (.pl0 files)
└── README.md           - This file
```

## Symbol Table

The compiler maintains a symbol table to track:
- Variable declarations
- Procedure declarations
- Array sizes
- Scope depth

The symbol table is implemented as a linked list (`struct symtab`) accessed through an encapsulated interface:
- `initsymtab()` - Initialize the symbol table
- `addsymbol()` - Add a new symbol
- `symcheck()` - Check if a symbol exists and validate its usage
- `arraycheck()` - Check array bounds
- `get_symtab_head()` - Safe accessor for the symbol table head

## Code Quality Notes

### Fix Applied
The initial "multiple definition of `head`" linker error was resolved by:
1. Making `head` a static variable in `ast.c` (limiting its scope to that translation unit)
2. Providing a safe accessor function `get_symtab_head()` for external access
3. Removing the global declaration from the header file

This follows the principle of **encapsulation**: the symbol table is now private to the AST module, and external modules access it through a controlled interface.

## Testing

Test files are located in the `tests/` directory. Each `.pl0` file contains a PL/0 program that tests various language features.

Example test files:
- `0000.pl0` - Empty program
- `0001.pl0` - Comments test
- `0002.pl0` through `0008.pl0` - Various language features

## Compiler Output

The compiler generates output during compilation, reporting:
- Parse errors with line numbers
- Symbol table entries
- Code generation details
- Final program output

## Build Artifacts

The following files are automatically generated and ignored by git:
- `*.o` - Object files
- `*.d` - Dependency files
- `*.exe` - Executable file
- All other standard build artifacts (see `.gitignore`)

## Author

Part of the Specialisterne Compiler Course.
