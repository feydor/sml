#include "stmt.h"


namespace Ast{
	void
	RetStmt::exec()
	{
		throw expr_->eval();
	}

	RetStmt::~RetStmt()
	{
		delete expr_;
	}
}