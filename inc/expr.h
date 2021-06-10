#ifndef EXPR_H
#define EXPR_H
#include "token.h"
#include <stack>

// forward declarations
class SymTable;

enum ExprType {
    BINARY, GROUPING, LITERAL, UNARY, KEYWORD
};

enum class Val_t {
    NUM, STR, GROUP, BOOL, OP, NIL, IDENT
};

/* placed on the stack machine for eval */
struct Eval {
    Val_t val_type;
    std::string val;
};

struct Expr {
    ExprType type;
    Val_t val_type;
    std::string val;
    Expr *left;
    Token op; // TODO: Is Token op instantiated in every constructor?
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

    // Literal expr, ident
    Expr(std::string sym, ExprType type);

    void eval(Expr const *curr, std::stack<Eval> &stack,
            SymTable const &sym_table);
    void print_tree(Expr *curr, std::string &res);
    private:
    std::string eval_binary(Token const &tok, double a, double b);
    std::string eval_binary(Token const &tok, std::string a, std::string b);
    std::string eval_unary(Token const &tok, double a);
    void swap_with_sym(Eval &curr, SymTable const &sym_table);
    void sym_undefined_exit(Expr const *curr);
};

#endif
