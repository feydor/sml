# SMOL
A (wip) dynamic language and interpreter with recursive descent parser, AST (abstract syntax tree) walking evaluator, and some object-oriented features (primitives have built-in methods). Derivative, kluged up and a first half-serious attempt.

## Language definition
See [GRAMMER.txt](GRAMMER.txt).

## Build
1. Install meson and ninja (https://mesonbuild.com/).  
2. Install the [dependencies](#Dependencies).  
3. On linux and macos (not tested) run the following: 
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
# really slow, about 310 ms on my machine (use option -b when running)
fn pascal_calc(row, i) {
    if (i == 0 or i == row - 1) ret 1
    ret pascal_calc(row - 1, i - 1) + pascal_calc(row - 1, i)
}

pascal(15) # 1 14 91 364 1001 2002 3003 3432 3003 2002 1001 364 91 14 1
```
see more at [examples](examples/).

## Project structure
- src/
    - ast/
        - expr/ - lines that produce values when evaluated.
    - lexer/ - token definitions, smallest language constructs.
    - parser/ - creates ASTs recursively.
    - util/ - error handling, ANSI terminal support.
    - smol.cpp - the main entry point for REPL and file execution.

## Dependencies
LLVM, Cmake
