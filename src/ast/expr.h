#ifndef EXPR_H
#define EXPR_H
#include "token.h"
#include "value.h"
#include "sym.h"
#include <stack>

// forward declarations
class SymTable;

enum class Expr_t {
    BINARY, GROUPING, LITERAL, UNARY, IDENT
};

namespace Ast {
    class Literal;
    
    class Expr {
        Expr_t type;
        Expr *_left = nullptr;
        Expr *_right = nullptr;

        public:
            Expr(Expr_t type, Expr *left, Expr *right)
                : type(type), _left(left), _right(right) {};
            Expr *left() const;
            Expr *right() const;
            bool is_binary() const;
            bool is_unary() const;
            bool is_grouping() const;
            bool is_ident() const;
            bool is_literal() const;
            std::string to_string(bool b);
    };

    class Binary : public Expr {
        Token _op;

        public:
            Binary(Expr *left, Token op, Expr *right)
                : Expr(Expr_t::BINARY, left, right)
                , _op(op) {};
            Token op();
            static Val::Val eval(Val::Val const &a,
                    Token const &op, Val::Val const &b);
    };
    
    class Unary : public Expr {
        Token _op;

        public:
            Unary(Token op, Expr *right)
                : Expr(Expr_t::UNARY, nullptr, right)
                , _op(op) {};
            Token op();
            static Val::Val eval(Token const &op, Val::Val const &a);
    };

    class Grouping : public Expr {
        public:
            Grouping(Expr *expr)
                : Expr(Expr_t::GROUPING, expr, nullptr) {};
    };

    class Ident : public Expr {
        Sym _sym;

        public: 
           Ident(Sym sym)
               : Expr(Expr_t::IDENT, nullptr, nullptr)
               , _sym(sym) {};
            
           Sym sym();
    };

    class Literal : public Expr {
        Val::Val _val;

        public:
            Literal(Val::Val val)
                : Expr(Expr_t::LITERAL, nullptr, nullptr)
                , _val(val) {};
           
            Val::Val val();
    };
}
#endif
