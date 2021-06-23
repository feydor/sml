# SMOL
A (wip) dynamic language and interpreter, recursive descent parser, AST (abstract syntax tree) walking evaluator.

## Language definition
See [GRAMMER.txt](GRAMMER.txt)
## Project structure
- src/
    - ast/
        - stmt/ - lines that do not produce values when executed.
        - expr/ - lines that produce values when evaluated.
    - env/ - classes for identifier resolution and for storing return values from user-defined functions.
    - lexer/ - token definitions, smallest language constructs.
    - lib/ - the foreign function interface (FFI) for user-defined functions and the (wip) prelude.
    - parser/ - creates ASTs recursively.
    - util/ - the definition of a SMOL value, (wip) ANSI terminal support for errors.
    - smol.cpp - the main entry point for REPL and file execution.

## Build
```sh
meson build
ninja -C build
./build/smol examples/fn.smol
```
## Examples
The below code fragment is an example written in smol and demonstrates what is so far implemented:
```
# some math demonstrating working operator precedence,
# symbol resolution, dynamic typing, etc
let x = (-23 - (8 + (-12) * 2 - 1 - 12)) / (-21 - (-20)) # x = -6
let y = x + 1
say x + y # prints -11

# also fizzbuzz with formatting
fn fizzbuzz() {
    say "[1] "
    for (let i = 1; i <= 100; i++) {
        if (i % 15 == 0)
            say "fizzbuzz "
        elif (i % 3 == 0)
            say "fizz "
        elif (i % 5 == 0)
            say "buzz "
        else
            say to_str(i) + " "

        # every five numbers, start a new row
        if (i % 5 == 0)
            say "\n[" + to_str(i + 1) + "] "
    }
}

fizzbuzz()
```
