/* userfn.cpp - user-defined functions */
#include "ffi.h"
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
std::shared_ptr<Obj::Object>
UserFn::invoke(std::vector<std::shared_ptr<Obj::Object>> args)
{
    check_nargs(args, argnames_.size());

	Env::push_back();
	for (auto i = 0; i < (int) args.size(); ++i)
		Env::set_var(argnames_[i], args[i]);

	// std::cout << FnTable::to_str() << std::endl;
	// std::cout << Env::to_str() << std::endl;
	try {
		body_->exec();
	} catch (std::shared_ptr<Obj::Object>& rval) {
		Env::pop();
		return rval;
	}
	/* unreached */
	return nullptr;
}

UserFn::~UserFn()
{
	delete body_;
}

