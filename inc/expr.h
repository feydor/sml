#ifndef EXPR_H
#define EXPR_H
#include "token.h"
#include <variant>

enum ExprType {
    BINARY, GROUPING, LITERAL, UNARY
};

class Expr {
    ExprType type;
};

class Binary : Expr {
    Expr &left;
    Token &op;
    Expr &right;

    public:
    Binary(Expr &left, Token &op, Expr &right)
        : left(left), op(op), right(right) {
        this->type = BINARY;    
    };
};

class Grouping : Expr {
    Expr &expr;

    public:
    Grouping(Expr &expr)
        : expr(expr) {
        this->type = GROUPING;
    };
};

class Literal : Expr {
    std::variant<double, std::string> val;

    public:
    Literal(double dbl)
        : val(dbl) {
        this->type = LITERAL;
    };
    Literal(std::string str)
        : val(str) {
        this->type = LITERAL;
    };
};


class Unary : Expr {
    Token &op;
    Expr &right;

    public:
    Unary(Token &op, Expr &right)
        : op(op), right(right) {
        this->type = UNARY;
    };
};

#endif
