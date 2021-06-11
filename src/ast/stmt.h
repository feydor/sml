#ifndef STMT_H
#define STMT_H
#include <string>

enum class Stmt_t { VAR_DECL, SAY, EXPR };

namespace Ast {
    struct Expr;
    /* A wrapper around an expr so that they can be called and assigned
     * to variables and functions
     * Possible types:
     * - variable declaration without definition
     * - variable declaration with definition
     * - say statement (print)
     * - expression statement
     */
    struct Stmt {
        Stmt_t type;
        Ast::Ident *ident; // can be a keyword, or a user-defined identifier
        Ast::Expr *expr;

        static std::string type_to_string(Stmt_t type);

        // identifier with definition or keyword with expression
        Stmt(Stmt_t type, Ast::Expr *ident, Ast::Expr *expr)
            : type(type), ident(ident), expr(expr) {};

        // identifier without definition
        Stmt(Stmt_t type, Ast::Expr *ident)
            : type(type), ident(ident), expr(nullptr) {};

        // EXPR_STMT, expression without identifier
        Stmt(Stmt_t type, Ast::Expr *expr, bool unused)
            : type(type), ident(nullptr), expr(expr) { (void)unused; };
    };
}

#endif

