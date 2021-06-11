#ifndef EXPR_H
#define EXPR_H
#include "token.h"
#include "value.h"
#include <stack>

// forward declarations
class SymTable;

enum class Expr_t {
    BINARY, GROUPING, LITERAL, UNARY, IDENT,
};

namespace Ast {
    class Expr {
        Expr_t type;
        Expr *left = nullptr;
        Expr *right = nullptr;

        public:
            Expr(Expr_t type, Expr *left, Expr *right)
                : type(type), left(left), right(right) {};
            Expr *left();
            Expr *right();
            Val::Val eval() = 0;
            void eval_ast(Expr const *curr, std::stack<Val::Val> &stack,
                    SymTable const &sym_table);
            void swap_with_sym(Val::Val &curr, SymTable const &sym_table);
            void sym_undefined_exit(Expr const *curr);
            std::string to_string(bool b);
            bool is_binary();
            bool is_unary();
            bool is_grouping();
            bool is_ident();
            bool is_literal();
            bool is_op();
            bool is_keyword();
    };

    class Binary : public Expr {
        Token op;

        public:
            Binary(Expr *left, Token op, Expr *right)
                : Expr(Expr_t::BINARY, left, right)
                , op(op) {};
            Token op();

            // does op on left and right
            Val::Val eval() override;
    };
    
    class Unary : public Expr {
        Token op;

        public:
            Unary(Token op, Expr *right)
                : Expr(Expr_t::UNARY, nullptr, right)
                , op(op) {};
            Token op();

            // does op on right
            Val::Val eval() override;
    };

    class Grouping : public Expr {
        public:
            Grouping(Expr *expr)
                : Expr(Expr_t::GROUPING, expr, nullptr) {};

            // calls eval of left
            Val::Val eval() override;
    };

    class Ident : public Expr {
        Sym sym;

        public: 
           Ident(Sym sym)
               : Expr(Expr_t::IDENT, nullptr, nullptr)
               , sym(sym) {};
            
           // returns nil val
           Val::Val eval() override;
    };

    class Literal : public Expr {
        Val::Val val;

        public:
            Literal(Val::Val val)
                : Expr(Expr_t::LITERAL, nullptr, nullptr)
                , val(val) {};
           
            // returns val
            Val::Val eval() override;
    };
}
#endif
