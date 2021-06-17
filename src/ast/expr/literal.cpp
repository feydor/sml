#include "expr.h"
using namespace Ast;

Val::Val
Literal::val() const
{
    return this->_val;
}

std::string
Literal::to_str() const
{
    return val().to_str();
}
