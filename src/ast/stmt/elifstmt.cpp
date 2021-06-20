#include "stmt.h"

namespace Ast {
	void
	ElIfStmt::exec()
	{
		if (cond_->eval().is_truthy())
			body_->exec();
	}

	ElIfStmt::~ElIfStmt()
	{
		delete cond_;
		delete body_;
	}
}