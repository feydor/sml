#include "stmt.h"
#include "env.h"
#include "expr.h"
#include "object.h"

namespace Ast {
	AsgmtStmt::AsgmtStmt(const std::string& name, std::unique_ptr<Ast::Expr> expr)
    	: name_(name), expr_(std::move(expr)) {};

	void
	AsgmtStmt::exec()
	{
		Env::set_var(name_, expr_->eval());
	}
}
