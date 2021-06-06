#ifndef EXPR_H
#define EXPR_H
#include "token.h"
#include <variant>

enum ExprType {
    BINARY, GROUPING, LITERAL, UNARY,
    EXPR_NUMBER, EXPR_STRING, EXPR_BOOL
};

struct Expr {
    ExprType type;
    Expr *left; // used for grouping
    Token op;
    Expr *right;
    std::variant<double, std::string, bool> val;

    Expr(Expr *left, Token op, Expr *right)
        : type(BINARY), left(left), op(op), right(right) {};
    Expr(Token op, Expr *right)
        : type(UNARY), left(nullptr), op(op), right(right) {};
    Expr(Expr *expr)
        : type(GROUPING), left(expr), right(nullptr) {};
    Expr(double dbl) : type(EXPR_NUMBER), val(dbl) {};
    Expr(std::string s) : type(EXPR_STRING), val(s) {};
    Expr(bool b) : type(EXPR_BOOL), val(b) {};
    Expr() {};

};

#endif
