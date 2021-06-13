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
    switch (op.type) {
        case MINUS:
            if (a.is_num())
                return Val::Val(a.get_num() - b.get_num());
            break;
        case PLUS:
            if (a.is_num())
                return Val::Val(a.get_num() + b.get_num());
            else if (a.is_str())
                return Val::Val(a.get_str() + b.get_str());
            break;
        case SLASH:
            if (a.is_num())
                return Val::Val(a.get_num() / b.get_num());
            break;
        case STAR:
            if (a.is_num())
                return Val::Val(a.get_num() * b.get_num());
            break;
        default: 
            // SMOL::error(op, "BINARY::eval: Unimplemented operator.");
            std::cout << "BINARY:eval: Unimplemented operator.\n";
            return Val::Val();
            break;
    }  
    /* not reached */
    return Val::Val();
}
