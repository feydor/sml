/* userfn.cpp - user-defined functions */
#include "ffi.h"
#include "stmt.h"
#include "env.h"
#include "fntable.h"
#include "retstack.h"
#include <iostream>

void
UserFn::set_body(Ast::Stmt* body)
{
	body_ = body;
}

void
UserFn::add_argname(std::string name)
{
	argnames_.push_back(name);
}

std::string
UserFn::name()
{
	return name_;
}

/* setups an environment for the user-defined function call
 * and passes control to its exec function
 */
Val
UserFn::invoke(std::vector<Val> args)
{
    check_nargs(args, argnames_.size());

	Env::push_back();
	for (auto i = 0; i < (int) args.size(); ++i)
		Env::set_var(argnames_[i], args[i]);

	// std::cout << Env::to_str() << std::endl;
	// std::cout << FnTable::to_str() << std::endl;
	body_->exec();
	auto ret_val = RetStack::pop();
	Env::pop();
	return ret_val; // TODO: Find a way to get values out of fn,
	              // through return statement, for now return NIL value
}

UserFn::~UserFn()
{
	delete body_;
}

