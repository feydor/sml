#include "sym.h"

Sym_t
Sym::type() const
{
    return type_;
}

std::string
Sym::sym() const
{
    return sym_;
}

std::string
Sym::to_str() const
{
    return sym_;
}

Val::Val
Var::val() const
{
    return val_;
}

