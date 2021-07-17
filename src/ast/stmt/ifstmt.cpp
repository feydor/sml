#include "stmt.h"
#include "expr.h"
#include "object.h"

namespace Ast {
	IfStmt::IfStmt(std::unique_ptr<Ast::Expr> cond, std::unique_ptr<Ast::Stmt> body)
        : cond_(std::move(cond)), body_(std::move(body)) {};

	void
	IfStmt::exec()
	{
		if (cond_->eval()->is_truthy())
			body_->exec();
		else if (elses_.size() > 0)
			for (const auto& els : elses_)
				els->exec();
	}

    void
    IfStmt::add_else(std::unique_ptr<Ast::Stmt> els)
    {
    	elses_.push_back(std::move(els));
    }
}
