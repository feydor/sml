/* userfn.cpp - user-defined functions */
#include "fn.h"
#include "stmt.h"
#include "env.h"
#include "fntable.h"
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
	if (args.size() != argnames_.size())
		throw std::runtime_error(
			"The number of parameters to this function is " +
			std::to_string(argnames_.size()) + ". But you passed in " +
			std::to_string(args.size()) + " arguments.");

	Env::push_back();
	for (auto i = 0; i < (int) args.size(); ++i)
		Env::set_var(argnames_[i], args[i]);

	// std::cout << Env::to_str() << std::endl;
	//std::cout << FnTable::to_str() << std::endl;
	body_->exec();
	Env::pop();
	return Val("END");
	// return Val(); // TODO: Find a way to get values out of fn,
				  // through a return statement
}

UserFn::~UserFn()
{
	delete body_;
}
