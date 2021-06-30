#include "expr.h"
#include <iostream>

namespace Ast {
std::shared_ptr<Obj::Object>
Unary::eval()
{
    auto x = right_->eval();

    switch (op_.type()) {

        case Token::MINUS: return -*x;
        case Token::BANG: return !*x;
        default:
            throw std::runtime_error("Unary: Unexpected operator. " + op_.to_str());
    }
    /* not reached */
    return nullptr;
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
