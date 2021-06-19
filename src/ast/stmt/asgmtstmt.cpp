#include "stmt.h"

namespace Ast {
	void
	AsgmtStmt::exec()
	{
		VarTable::set_var(name_, expr_->eval());
	}

	AsgmtStmt::~AsgmtStmt()
	{
		delete expr_;
	}
}