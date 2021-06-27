#include "tostr.h"
#include <iostream>

namespace Lib {
	std::string
	to_str::name()
	{
		return "to_str";
	}

	Val
	to_str::invoke(std::vector<Val> args)
	{
		check_nargs(args, 1);
		return args[0].to_str();
	}
}