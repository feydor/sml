#include "expr.h"
#include <sstream>
#include <stdlib.h>
#include <assert.h>

namespace Ast {
	std::shared_ptr<Obj::Object>
	Arr::eval()
	{
		std::vector<std::shared_ptr<Obj::Object>> arr;
		for (auto& expr : exprs_)
			arr.push_back(expr->eval());
		return std::make_unique<Obj::Array>(arr);
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
