#include "stmt.h"
#include <iostream>

namespace Ast {

	void
	IfStmt::exec()
	{
		if (cond_->eval().is_truthy())
			body_->exec();
		else if (elses_.size() > 0)
			for (auto& els : elses_)
				els->exec();
	}

    void
    IfStmt::add_else(Stmt* els)
    {
    	elses_.push_back(els);
    }

	IfStmt::~IfStmt()
	{
		delete cond_;
		delete body_;
		for (auto& els : elses_)
			delete els;
	}
}