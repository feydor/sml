#include "exit.h"
#include <stdlib.h>

namespace Lib {
	std::string
	smol_exit::name()
	{
		return "exit";
	}

	std::shared_ptr<Obj::Object>
	smol_exit::invoke(std::vector<std::shared_ptr<Obj::Object>> args)
	{
		check_nargs(args, 1);
		auto exit_code = std::dynamic_pointer_cast<Obj::Number>(args[0])->num();
		// throw exit_code;
		exit(exit_code);
	}
}
