#ifndef STMT_H
#define STMT_H
#include <string>
#include <vector>

// forward declarations

enum class Stmt_t { VAR_DECL, VAR_DEF, VAR_REDEF,
    BLOCK, COND, SAY, EXPR };

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
        Ast::Expr *expr_;

        public:
        Stmt(Stmt_t type, Ast::Expr *expr)
            : type(type), expr_(expr) {};

        Expr * expr();
        bool is_say_stmt();
        bool is_expr_stmt();
        bool is_decl_stmt();
        bool is_def_stmt();
        bool is_redef_stmt();
        bool is_block_stmt();
        bool is_cond_stmt();
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
        Ident *ident_;
        public:
        // identifier with definition
        IdentStmt(Ast::Ident *ident, Ast::Expr *def)
            : Stmt(Stmt_t::VAR_DEF, def), ident_(ident) {};

        // identifier without definition (declaration)
        IdentStmt(Ast::Ident *ident)
            : Stmt(Stmt_t::VAR_DECL, nullptr), ident_(ident) {};
        
        // identifier with redefinition
        // must be declared at runtime else runtime error
        IdentStmt(Ast::Ident *ident, Ast::Expr *def, bool redef)
            : Stmt(Stmt_t::VAR_REDEF, def), ident_(ident) { (void)redef; };
        
        Ident * ident();
    };

    class BlockStmt : public Stmt {
        std::vector<Stmt *> stmts;

        public:
        BlockStmt()
            :Stmt(Stmt_t::BLOCK, nullptr) {};

        void add_stmt(Stmt *stmt);
        std::vector<Stmt *> get_stmts();
    };

    class ElseStmt : public Stmt {
        Stmt *body_;

        public:
        ElseStmt(Stmt *body)
            : Stmt(Stmt_t::COND, nullptr), body_(body) {};
        Stmt *body() const;
    };

    class ElifStmt : public Stmt {
        Expr *cond_;
        Stmt *body_;
        ElseStmt *else_;

        public:
        ElifStmt(Expr *cond, Stmt *body)
            : Stmt(Stmt_t::COND, cond)
            , cond_(cond), body_(body) {};
        void set_else(ElseStmt *els);
    };

    class IfStmt : public Stmt {
        Expr *cond_;
        Stmt *body_;
        
        std::vector<Stmt *> elifs_;
        ElseStmt *else_ = nullptr;

        public:
        IfStmt(Expr *cond, Stmt *body)
            : Stmt(Stmt_t::COND, cond)
            ,cond_(cond), body_(body) {};

        int else_or_elifs() const;
        void add_elif(ElifStmt *elif);
        void set_else(ElseStmt *els);
        Expr *cond() const;
        Stmt *body() const;
        ElseStmt *else_stmt() const;
        std::vector<Stmt *> elifs() const;
    };
}

#endif

