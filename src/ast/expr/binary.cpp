#include "expr.h"
#include "object.h"
#include <iostream>

namespace Ast {

Binary::Binary(std::unique_ptr<Expr> left, Tok op, std::unique_ptr<Expr> right)
    : op_(op), left_(std::move(left)), right_(std::move(right)) {};

std::shared_ptr<Obj::Object>
Binary::eval()
{
    auto left = left_->eval();
    auto right = right_->eval();

    switch (op_.type()) {
        case Token::MINUS: return *left - *right;
        case Token::PLUS: return *left + *right;
        case Token::SLASH: return *left / *right;
        case Token::STAR: return *left * *right;
        case Token::PERCENT: return *left % *right;
        case Token::LEFT_BRACKET: return (*left)[*right];
        case Token::DOT: return left->dot_operator(*right);
        default:
            throw std::runtime_error("Operator '" + op_.to_str() + "' is unimplemented.");
    }
}

std::string
Binary::to_str() const
{
    return left_->to_str() + " " + op_.to_str() + " " + right_->to_str();
}

}
