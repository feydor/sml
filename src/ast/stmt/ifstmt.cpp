#include "stmt.h"

namespace Ast {

	void
	IfStmt::exec()
	{
		if (cond_->eval().get_bool())
			ifstmt_->exec();
		else if (elsestmt != nullptr)
			elsestmt_->exec();
	}


	IfStmt::~IfStmt()
	{
		delete cond_;
		delete ifstmt_;
		delete elsestmt_;
	}
}