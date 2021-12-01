# SMOL
A (wip) dynamic language with recursive descent parser, LLVM IR code generations, and JIT compilation; kluged up and a first half-serious attempt.

## Try it
[smol-lang.netlify.app](https://smol-lang.netlify.app)

## Build
1. Install meson and ninja (https://mesonbuild.com/).  
2. Install the [dependencies](#Dependencies).  
3. On linux and macos (not tested) run the following: 
```sh
meson build
ninja -C build
./build/smol examples/0.smol
```
## Examples
The below code fragment is an example written in smol and demonstrates what is so far implemented:

```python
extern sin(x)
extern cos(x)
def square(x) x*x
def foo(x)
  square(sin(x)) + square(cos(x))

foo(4.0) # 1.0
```
see more at [examples](examples/).

## Project structure
- src/
    - ast/
        - expr/ - evaluatable expressions
    - lexer/ - token definitions, smallest language constructs.
    - parser/ - creates ASTs recursively.
    - util/ - error handling, ANSI terminal support.
    - smol.cpp - the main entry point for REPL and file execution.

## Dependencies
LLVM 12, Cmake
