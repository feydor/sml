#ifndef EXPR_H
#define EXPR_H
#include "token.h"
#include <stack>
#include <variant>

enum ExprType {
    BINARY, GROUPING, LITERAL, UNARY, KEYWORD,
};

enum val_t {
    VAL_NUMBER, VAL_STRING, VAL_GROUP, VAL_BOOL, VAL_OP, VAL_NIL
};

/* placed on the stack machine for eval */
struct Eval {
    val_t val_type;
    std::string val;
};

struct Expr {
    ExprType type;
    val_t val_type;
    std::string val;
    Expr *left;
    Token op;
    Expr *right;

    // Binary expr
    Expr(Expr *left, Token op, Expr *right);

    // Unary expr
    Expr(Token op, Expr *right);

    // Grouping expr
    Expr(Expr *expr);

    // Literal expr, number
    Expr(double num);

    // Literal expr, string
    // used for user-defined identifers/syms
    Expr(std::string ident);

    // Literal expr, bool
    Expr(bool b);

    // Literal expr, keyword
    Expr(std::string keyword, TokenType type);

    void eval(Expr *curr, std::stack<Eval> &stack);
    void print_tree(Expr *curr, std::string &res);
    private:
    std::string eval_binary(Token const &tok, double a, double b);
    std::string eval_binary(Token const &tok, std::string a, std::string b);
    std::string eval_unary(Token const &tok, double a);
};

#endif
