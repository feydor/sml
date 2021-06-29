#include "expr.h"
#include <iostream>
#include <functional>

namespace Ast {
Obj::Object*
Cond::eval()
{
	auto left = left_->eval();
	auto right = right_->eval();

	switch (op_.type()) {
		case Token::AND: return *left && *right;
        case Token::OR: return *left || *right;
        case Token::BANG_EQUAL: return *left != *right;
        case Token::EQUAL_EQUAL: return *left == *right;
        case Token::LESS: return *left < *right;
        case Token::LESS_EQUAL: return *left <= *right;
        case Token::GREATER: return *left > *right;
        case Token::GREATER_EQUAL: return *left >= *right;
        default: throw std::runtime_error("Cond: Unknown operator.");
	}
}

Cond::~Cond()
{
	delete left_;
	delete right_;
}

std::string
Cond::to_str() const
{
	return left_->to_str() + " " + op_.to_str() + " " + right_->to_str();
}

}
