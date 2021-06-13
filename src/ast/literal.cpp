#include "expr.h"
using namespace Ast;

Val::Val
Literal::val()
{
    return this->_val;
}
