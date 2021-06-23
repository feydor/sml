#include "stmt.h"
#include "retstack.h"

namespace Ast{
	void
	RetStmt::exec()
	{
		RetStack::push_back(expr_->eval());
	}

	RetStmt::~RetStmt()
	{
		delete expr_;
	}
}