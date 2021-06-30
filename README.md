# SMOL
A (wip) dynamic language and interpreter, recursive descent parser, AST (abstract syntax tree) walking evaluator. Derivative, kluged up and a first half-serious attempt: *'to err is human, to forgive divine.'*

## Language definition
See [GRAMMER.txt](GRAMMER.txt)
## Build
1. Install meson and ninja (https://mesonbuild.com/).
2. On linux and macos (not tested) run the following: 
```sh
meson build
ninja -C build
./build/smol examples/pascal.smol
```
## Examples
The below code fragment is an example written in smol and demonstrates what is so far implemented:
```python
# prints the nth row of Pascal's triangle... because why not?
fn pascal(row) {
    say "Pascal's triangle, row #" + to_str(row) + ": "
    for (let i = 0; i < row; i++)
        say pascal_calc(row, i) + " "
}

# just a basic tree recursion I did in SICP js once
# really slow, about 310 ms on my machine (using option -b when running)
fn pascal_calc(row, i) {
    if (i == 0 or i == row - 1)
        ret 1;
    ret pascal_calc(row - 1, i - 1) + pascal_calc(row - 1, i);
}

pascal(15) # 1 14 91 364 1001 2002 3003 3432 3003 2002 1001 364 91 14 1
```
## Project structure
- src/
    - ast/
        - stmt/ - lines that do not produce values when executed.
        - expr/ - lines that produce values when evaluated.
    - env/ - classes for identifier resolution and for storing return values from user-defined functions.
    - lexer/ - token definitions, smallest language constructs.
    - lib/ - the foreign function interface (FFI) for user-defined functions and the (wip) prelude.
    - parser/ - creates ASTs recursively.
    - util/ - error handling, (wip) ANSI terminal support.
        - object/ - the SMOL primitive definition.
    - smol.cpp - the main entry point for (wip) REPL and file execution.
## TODO
- wishlist: builtin functions such as getc(), putc(), fopen(), etc.
- wishlist: brainfsck interpreter in SMOL.
