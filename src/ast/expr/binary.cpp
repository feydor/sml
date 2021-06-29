#include "expr.h"
#include <iostream>

namespace Ast {

Obj::Object*
Binary::eval()
{
    Obj::Object* left = left_->eval();
    Obj::Object* right = right_->eval();

    switch (op_.type()) {
        case Token::MINUS: return *left - *right;
        case Token::PLUS: return *left + *right;
        case Token::SLASH: return *left / *right;
        case Token::STAR: return *left * *right;
        case Token::PERCENT: return *left % *right;
        case Token::LEFT_BRACKET: return (*left)[*right];
        default:
            throw std::runtime_error("Operator '" + op_.to_str() + "' is unimplemented.");
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
