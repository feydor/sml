#include "stmt.h"
//#include <iostream>

namespace Ast {
	// evaluate the expr and return
	void
	ExprStmt::exec()
	{
		// std::cout << expr_->to_str() << std::endl;
		expr_->eval();
	}

	ExprStmt::~ExprStmt()
	{
		delete expr_;
	}
}