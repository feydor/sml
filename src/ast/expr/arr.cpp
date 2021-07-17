#include "expr.h"
#include "object.h"
#include <sstream>
#include <stdlib.h>
#include <assert.h>

namespace Ast {
	Arr::Arr(std::vector<std::unique_ptr<Expr>> exprs)
		: exprs_(std::move(exprs)) {};

	std::shared_ptr<Obj::Object>
	Arr::eval()
	{
		std::vector<std::shared_ptr<Obj::Object>> arr;
		for (auto& expr : exprs_)
			arr.push_back(expr->eval());
		return std::make_shared<Obj::Array>(arr);
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
