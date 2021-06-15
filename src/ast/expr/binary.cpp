#include "expr.h"
#include <iostream>
using namespace Ast;

Token
Binary::op()
{
    return this->_op;
}

// assumes a and b have the same val type
Val::Val
Binary::eval(Val::Val const &a, Token const &op, Val::Val const &b)
{
    switch (op.get_type()) {
        case MINUS: return a - b;
        case PLUS: return a + b;
        case SLASH: return a / b;
        case STAR: return a * b;
        default: 
            // SMOL::error(op, "BINARY::eval: Unimplemented operator.");
            std::cout << "BINARY:eval: Unimplemented operator.\n";
            return Val::Val();
            break;
    }  
    /* not reached */
    return Val::Val();
}
