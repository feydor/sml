#include "stmt.h"

namespace Ast {
	// evaluate the expr and return
	void
	ExprStmt::exec()
	{
		expr_->eval();
	}

	ExprStmt::~ExprStmt()
	{
		delete expr_;
	}
}