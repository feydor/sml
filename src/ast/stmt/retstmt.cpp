#include "stmt.h"
#include "expr.h"
#include "object.h"

namespace Ast{
	RetStmt::RetStmt(std::unique_ptr<Ast::Expr> expr)
        : expr_(std::move(expr)) {};

	void
	RetStmt::exec()
	{
		throw expr_->eval();
	}
}
