#include "stmt.h"
#include <iostream>

namespace Ast {

	void
	IfStmt::exec()
	{
		if (cond_->eval().is_truthy())
			body_->exec();
		else if (elsestmt_ != nullptr)
			elsestmt_->exec();
	}

	void
	IfStmt::set_else(Stmt* elsestmt)
	{
		elsestmt_ = elsestmt;
	}


    void
    IfStmt::add_elif(Stmt* elif)
    {
    	elifs_.push_back(elif);
    }

	IfStmt::~IfStmt()
	{
		delete cond_;
		delete body_;
		if (elsestmt_ != nullptr)
			delete elsestmt_;
		for (auto& elif : elifs_)
			delete elif;
	}
}