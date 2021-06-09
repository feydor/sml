#ifndef STMT_H
#define STMT_H
#include "expr.h"

enum stmt_t { VAR_DECL, SAY_STMT, EXPR_STMT };

// TODO: Split these into three classes
struct Stmt {
    stmt_t type;
    Expr *ident; // can be a keyword, or a user-defined identifier
    Expr *expr;
    
    // identifier with definition or keyword with expression
    Stmt(stmt_t type, Expr *ident, Expr *expr)
        : type(type), ident(ident), expr(expr) {};
    
    // identifier without definition
    Stmt(stmt_t type, Expr *ident)
        : type(type), ident(ident), expr(nullptr) {};
    
    // EXPR_STMT, expression without identifier
    Stmt(stmt_t type, Expr *expr, bool unused)
        : type(type), ident(nullptr), expr(expr) { (void)unused; };
};

#endif

