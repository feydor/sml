#include "expr.h"
#include <iostream>

namespace Ast {
Val
Unary::eval()
{
    Val x = right_->eval();

    switch (op_.type()) {
        case Token::MINUS:
            if (x.is_num())
                return Val(-(x.get_num()));
            break;
        case Token::BANG:
            if (x.is_bool())
                return Val(!(x.get_bool()));
            break;
        default: 
            std::cout << "Unary:eval: Unexpected operator. " + op_.to_str();
            return Val(); // nil
    }
    /* not reached */
    return Val();
}

Unary::~Unary()
{
    delete right_;
}

std::string
Unary::to_str() const
{
    return op_.to_str() + right_->to_str();
}
}