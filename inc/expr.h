#ifndef EXPR_H
#define EXPR_H
#include "token.h"
#include "<variant>"
class Expr {}
class Binary : Expr {
    Expr left;
    Token op;
    Expr right;

public:
     Binary(Expr left, Token op, Expr right)
    : left(left), op(op), right(right) {};
};
class Grouping : Expr {
    Expr expr;

public:
     Grouping(Expr expr)
    : expr(expr) {};
};
class Literal : Expr {
    Placeholder val;

public:
     Literal(Placeholder val)
    : val(val) {};
};
class Unary : Expr {
    Token op;
    Expr right;

public:
     Unary(Token op, Expr right)
    : op(op), right(right) {};
};
