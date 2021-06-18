#include "expr.h"
#include <iostream>
#include <functional>

namespace Ast {
Val
Cond::eval()
{
	Val left = left_->eval();
	Val right = right_->eval();

	switch (op_.type()) {
		case AND: return eval_(left, right_, std::logical_and());
        case OR: return eval_(left, right, std::logical_or());
        case BANG_EQUAL: return eval_(left, right, std::not_equal_to());
        case EQUAL_EQUAL: return eval_(left, right, std::equal_to());
        case LESS: return eval_(left, right, std::less());
        case LESS_EQUAL: return eval_(left, right, std::less_equal());
        case GREATER: return eval_(left, right, std::greater());
        case GREATER_EQUAL: return eval_(left, right, std::greater_equal());
        default:
        	std::cout << "Cond::Eval: Unknown operator '" +
        		op_.to_str() + "'." << std::endl;
        	return Val(); // nil
	}
}

// conditional operators are only effective on num and bool types
// return NIL on any other types
template<typename Op>
Val::Val
Cond::eval_(Val const &a, Val const &b, Op fn) {
    if (a.is_num()) {
        if (b.is_num())
            return Val::Val(fn(a.get_num(), b.get_num()));
        else if (b.is_bool())
            return Val::Val(fn(a.get_num(), b.get_bool()));
    } else if (a.is_bool()) {
        if (b.is_num())
            return Val::Val(fn(a.get_bool(), b.get_num()));
        else if (b.is_bool())
            return Val::Val(fn(a.get_bool(), b.get_bool()));
    } else {
    	std::cout << "Cond::Eval: Attempted logical operation on non-num/bool\n";
  		return Val(); // nil
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
