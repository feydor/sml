#ifndef STMT_H
#define STMT_H
#include <string>

// forward declarations

enum class Stmt_t { VAR_DECL, VAR_DEF, SAY, EXPR };

namespace Ast {
    class Expr;
    class Ident;
    /* A wrapper around an expr so that they can be called and assigned
     * to variables and functions
     * Possible types:
     * - variable declaration without definition
     * - variable declaration with definition
     * - say statement (print)
     * - expression statement
     */
    class Stmt {
        Stmt_t type;
        Ast::Expr *_expr;

        public:
        Stmt(Stmt_t type, Ast::Expr *expr)
            : type(type), _expr(expr) {};

        Expr * expr();
        bool is_say_stmt();
        bool is_expr_stmt();
        bool is_decl_stmt();
        bool is_def_stmt();
        static std::string type_to_string(Stmt const *stmt);
    };

    class ExprStmt : public Stmt {
        public:
        ExprStmt(Ast::Expr *expr)
            : Stmt(Stmt_t::EXPR, expr) {};
    };

    class SayStmt : public Stmt {
        public:
        SayStmt(Ast::Expr *expr)
            : Stmt(Stmt_t::SAY, expr) {};
    };

    class IdentStmt : public Stmt {
        Ident *_ident;
        public:
        // identifier with definition
        IdentStmt(Ast::Ident *ident, Ast::Expr *def)
            : Stmt(Stmt_t::VAR_DEF, def), _ident(ident) {};

        // identifier without definition (declaration)
        IdentStmt(Ast::Ident *ident)
            : Stmt(Stmt_t::VAR_DECL, nullptr), _ident(ident) {};
        
        Ident * ident();
    };
}

#endif

