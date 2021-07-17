#include "getchar.h"
#include <stdio.h>

namespace Lib {
	std::string
	smol_getchar::name()
	{
		return "getchar";
	}

	std::shared_ptr<Obj::Object>
	smol_getchar::invoke(std::vector<std::shared_ptr<Obj::Object>> args)
	{
		check_nargs(args, 0);
		char c = getchar();
		return std::make_shared<Obj::Number>(c);
	}
}
