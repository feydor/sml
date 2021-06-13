#include "expr.h"

namespace Ast {
std::string
Expr::to_string(bool b)
{
    return b ? "true" : "false";
}

Expr *
Expr::left() const
{
    return this->_left;
}

Expr *
Expr::right() const
{
    return this->_right;
}

bool 
Expr::is_binary() const
{
    return this->type == Expr_t::BINARY;
}

bool 
Expr::is_unary() const
{
    return this->type == Expr_t::UNARY;
}

bool 
Expr::is_grouping() const
{
    return this->type == Expr_t::GROUPING;
}

bool 
Expr::is_ident() const
{
    return this->type == Expr_t::IDENT;
}

bool 
Expr::is_literal() const
{
    return this->type == Expr_t::LITERAL;
}

}
