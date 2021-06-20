#include "tostr.h"

namespace Lib {
	std::string
	to_str::name()
	{
		return "to_str";
	}

	Val
	to_str::invoke(std::vector<Val> args)
	{
		check_args(args, 1);
		return args[0].to_str();
	}
}