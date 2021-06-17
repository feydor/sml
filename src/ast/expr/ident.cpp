#include "expr.h"
using namespace Ast;
           
Sym
Ident::sym() const
{
    return _sym;
}

std::string
Ident::to_str() const
{
    return sym().to_str();
}
