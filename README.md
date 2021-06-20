# SMOL
A wip smol (stack machine ordered language) lang and interpreter, recursive descent parser, AST (abstract syntax tree) walking evaluator.

## Language definition
See [GRAMMER.txt](GRAMMER.txt)
## Project structure
- inc
    - stmt.h - a wrapper around an AST root; directly interpreted by intpr; declarations resolved at this level.
    - expr.h - AST nodes that can be depth-first evaluated
    - sym.h - A table holding the running program's currently defined symbols (vars or functions).
    - token.h - keywords, literals, smallest language constructs

## Examples
The below code fragment is an example written in smol and demonstrates what is so far implemented:
```
# some math demonstrating working operator precedence,
# symbol resolution, dynamic typing, etc
let x = (-23 - (8 + (-12) * 2 - 1 - 12)) / (-21 - (-20)) # x = -6
let y = x + 1
say x + y # prints -11

# also fizzbuzz with formatting
let i = 1
say "[" + to_str(i) + "] "
while (i <= 100) {
    if (i % 15 == 0)
        say "fizzbuzz "
    elif (i % 3 == 0)
        say "fizz "
    elif (i % 5 == 0)
        say "buzz "
    else
        say to_str(i) + " "
    if (i % 5 == 0)
        say "\n[" + to_str(i + 1) + "] "
    i = i + 1
}
```
