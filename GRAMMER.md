## Precedence (from lowest to highest)
- equality, '== !=', left
- comparison, '> >= <= <', left
- term, '= +', left
- factor, '/ *', left
- unary, '! -', right

## Grammer, Productions
- program     -> statement* EOF;
- statement   -> expr_stmt | say_stmnt;
- expr_stmt   -> expression"\n";
- say_stmt    -> "say" expression"\n";

- expression  -> equality;
- equality    -> comparison (("!=" | "==") comparison)*;
- comparison  -> term ((">" | ">=" | "<=" | "<") term)*;
- term        -> factor (("-") | "+") factor)*;
- factor      -> unary (("/" | "*") unary)*;
- unary       -> ("!" | "-") unary | primary;
- primary     -> NUMBER | STRING | "true" | "false" | "nil" | "("expression")";

