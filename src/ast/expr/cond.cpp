#include "expr.h"
#include "object.h"
#include <iostream>
#include <functional>

namespace Ast {
	Cond::Cond(std::unique_ptr<Expr> left, Tok op, std::unique_ptr<Expr> right)
        : left_(std::move(left)), op_(op), right_(std::move(right)) {};

	std::shared_ptr<Obj::Object>
	Cond::eval()
	{
		// left and right are deleted here
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

	std::string
	Cond::to_str() const
	{
		return left_->to_str() + " " + op_.to_str() + " " + right_->to_str();
	}

}
