#ifndef EXPR_H
#define EXPR_H
#include "token.h"
#include <stack>
#include <variant>

enum ExprType {
    BINARY, GROUPING, LITERAL, UNARY,
};
    
enum val_t { VAL_NUMBER, VAL_STRING, VAL_GROUP, VAL_BOOL, VAL_OP, VAL_NIL };

/* placed on the stack machine for eval */
struct Literal {
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

    Expr(Expr *left, Token op, Expr *right);
    Expr(Token op, Expr *right);
    Expr(Expr *expr);
    Expr(double dbl);
    Expr(std::string s);
    Expr(bool b);
    // Expr() {};

    void eval(Expr *curr, std::stack<Literal> &stack);
    private:
    std::string eval_binary(Token const &tok, double a, double b);
    std::string eval_binary(Token const &tok, std::string a, std::string b);
};

#endif
