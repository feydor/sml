#include "stmt.h"
#include "expr.h"
#include "object.h"
#include <iostream>

namespace Ast {
	ExprStmt::ExprStmt(std::unique_ptr<Ast::Expr> expr)
        : expr_(std::move(expr)) {}

	// evaluate the expr and return
	void
	ExprStmt::exec()
	{
		auto x = expr_->eval(); // return of eval dies here
		std::cout << x->to_str() << "\n";
	}
}
