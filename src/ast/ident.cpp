#include "expr.h"
using namespace Ast;
           
Sym
Ident::sym()
{
    return this->_sym;
}
