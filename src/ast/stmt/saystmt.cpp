#include "stmt.h"
#include <iostream>

namespace Ast {
	void
	SayStmt::exec()
	{
		std::cout << expr_->eval().to_str() << std::endl;
	}

	SayStmt::~SayStmt()
	{
		delete expr_;
	}
}