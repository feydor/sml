#include "stmt.h"

namespace Ast {

	void
	IfStmt::exec()
	{
		if (cond_->eval().get_bool())
			ifstmt_->exec();
		else if (else_stmt != nullptr)
			else_stmt_->exec();
	}


	IfStmt::~IfStmt()
	{
		delete cond_;
		delete ifstmt_;
		delete else_stmt_;
	}
}