#include "expr.h"
#include <iostream>
using namespace Ast;

Token
Unary::op() const
{
    return this->_op;
}

Val::Val
Unary::eval(Token const &op, Val::Val const &a)
{
    switch (op.type()) {
        case MINUS:
            if (a.is_num())
                return Val::Val(-(a.get_num()));
            break;
        case BANG:
            if (a.is_bool())
                return Val::Val(!(a.get_bool()));
            break;
        default: 
            std::cout << "Unary:eval: Unexpected operator. " + op.lexeme();
            return Val::Val();
            break;
    }
    /* not reached */
    return Val::Val();
}

std::string
Unary::to_str() const
{
    return op().to_str() + right()->to_str();
}