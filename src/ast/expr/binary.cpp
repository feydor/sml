#include "expr.h"
#include <iostream>

namespace Ast {

Val
Binary::eval()
{
    Val left = left_->eval();
    Val right = right_->eval();

    switch (op_.type()) {
        case Token::MINUS: return left - right;
        case Token::PLUS: return left + right;
        case Token::SLASH: return left / right;
        case Token::STAR: return left * right;
        // case Token::PERCENT: return left % right; // TODO: Add to token.type()
        default:
            // throw std::runtime_error(
            //    "Operator '" + op.to_str() + "' is unimplemented.");
            std::cout << "BINARY:eval: Unimplemented operator.\n";
            return Val(); // nil
    }
}

Binary::~Binary()
{
    delete left_;
    delete right_;
}

std::string
Binary::to_str() const
{
    return left_->to_str() + " " + op_.to_str() + " " + right_->to_str();
}

}
