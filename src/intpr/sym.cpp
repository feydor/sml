#include "sym.h"

Sym_t
Sym::type() const
{
    return this->type_;
}

std::string
Sym::sym() const
{
    return this->sym_;
}

Val::Val
Var::val() const
{
    return this->val_;
}

