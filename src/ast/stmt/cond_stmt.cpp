#include "stmt.h"

namespace Ast {

/**
 * returns 0 for else, 1 for elifs, -1 for neither 
 */
int
IfStmt::else_or_elifs() const
{
	return else_
	    ? 0
	    : elifs_.size() > 0
	    ? 1
	    : -1;
}

void
IfStmt::add_elif(ElifStmt *elif)
{
	elifs_.push_back((Stmt *)elif);
}

void
IfStmt::set_else(ElseStmt *els)
{
	else_ = els;
}

Expr *
IfStmt::cond() const
{
	return cond_;
}

Stmt *
IfStmt::body() const
{
	return body_;
}

ElseStmt *
IfStmt::else_stmt() const
{
	return else_;
}

std::vector<Stmt *>
IfStmt::elifs() const
{
	return elifs_;
}

void
ElifStmt::set_else(ElseStmt *els)
{
	else_ = els;
}

Stmt *
ElseStmt::body() const
{
	return body_;
}

}