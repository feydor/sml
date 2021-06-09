# SMOL
A wip smol interpreter, tree-walking for now.

## Language definition
See [GRAMMER.txt](GRAMMER.txt)
## Project structure
- inc
    - stmt.h - a wrapper around an AST root; directly interpreted by intpr; declarations live at this level.
    - expr.h - AST (abstract syntax tree) nodes that can be evaluated
    - sym.h - A table holding the running program's currently defined symbols (vars or functions). Smol supports function calling before declaration thanks to the symtable.
    - token.h - keywords, literals, smallest language constructs
