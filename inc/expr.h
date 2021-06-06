#ifndef EXPR_H
#define EXPR_H
#include "token.h"
#include <variant>

enum ExprType {
    BINARY, GROUPING, LITERAL, UNARY,
    EXPR_NUMBER, EXPR_STRING, EXPR_BOOL
};

/* returned by Expr::interpret */
struct Eval {
    double num;
    std::string str;
    bool b;
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
    Expr(double dbl) : type(EXPR_NUMBER), left(nullptr), right(nullptr), val(dbl) {};
    Expr(std::string s) : type(EXPR_STRING), left(nullptr), right(nullptr), val(s) {};
    Expr(bool b) : type(EXPR_BOOL), left(nullptr), right(nullptr), val(b) {};
    Expr() {};

    void to_string();
    double interpret(Expr *curr, Eval &res);
    double eval_binary(Expr *curr, Eval &res);
};

#endif
