#include "tostr.h"
#include <iostream>

namespace Lib {
	std::string
	to_str::name()
	{
		return "to_str";
	}

	Obj::Object*
	to_str::invoke(std::vector<Obj::Object*> args)
	{
		check_nargs(args, 1);
		return new Obj::String(args[0]->to_str());
	}
}
