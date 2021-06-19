#include "stmt.h"
#include "env.h"

namespace Ast {
	void
	AsgmtStmt::exec()
	{
		Env::set_var(name_, expr_->eval());
	}

	AsgmtStmt::~AsgmtStmt()
	{
		delete expr_;
	}
}