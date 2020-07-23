# CustomParser

## Overview
This project is a toy project to implement a simple top down parser with a custom syntax.
The algorithm is inspired by the structure of EBNF itself, meaning in order to parse a string successfully the following things are needed:

1. Handwritten parsing routines for terminal symbols (i.e. digits, letters, etc.).
2. For every symbol that is not purly non-terminal, parsing routines for the terminal parts have to be written.
3. All non-terminal symbols can be combined using the `all_of`, `one_of`, `optional` and `repeated` operators, which rougly correspond to the operators found in EBNF.



## Versions

The following features are planned for certain versions:

- **v0.1**\n
  Parsing of simple arithmetic expressions as described in `doc/v0.1.ebnf`. The goal here is to setup the needed helper functions, unit tests and get used to everything.

- **v0.2**\n
  Before progressing to the code generation part, the grammer is extended to support compound (parenthsized) expressions and simple builtin function calls (i.e. `sin()`, `cos()`, `tan()`). If there are any major flaws with the current approach those should be detected in this stage.

- **v0.3**\n
  For this version the error reporting should be improved as it is kind of non-existent by now. The report should consist of the line and column number of the error encountered as well as a sensible error message.

- **v0.4**\n
  By now a simple code generator should be implemented for the given syntax. Most probably the output will be in C++ or any other high level language. Code generators for assembler or LLVM-IL may be added later on.
