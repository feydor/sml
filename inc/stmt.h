#ifndef STMT_H
#define STMT_H
#include <string>

// forward declarations
struct Expr;

enum stmt_t { VAR_DECL, SAY_STMT, EXPR_STMT };

/* A wrapper around an expr so that they can be called and assigned
 * to variables and functions
 * Possible types:
 * - variable declaration without definition
 * - variable declaration with definition
 * - say statement (print)
 * - expression statement
 */
struct Stmt {
    stmt_t type;
    Expr *ident; // can be a keyword, or a user-defined identifier
    Expr *expr;

    static std::string type_to_string(stmt_t type);
    
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

