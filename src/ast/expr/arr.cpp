#include "expr.h"
#include <sstream>
#include <stdlib.h>
#include <assert.h>

namespace Ast {
	Val
	Arr::eval()
	{
		std::vector<Val> arr;
		for (auto& expr : exprs_)
			arr.push_back(expr->eval());
		return Val(arr);
	}

	Arr::~Arr()
	{
		for (auto& expr : exprs_)
			delete expr;
	}

	std::string
	Arr::to_str() const
	{
		std::stringstream ss;
		ss << "[";
		for (auto& expr : exprs_)
			ss << expr->to_str() << " ";
		ss << "]";
		return ss.str();
	}
};
