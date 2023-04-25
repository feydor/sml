#include "help.h"
#include <string>
#include <memory>
#include <vector>

namespace Lib {
	std::string
	help::name()
	{
		return "help";
	}

	std::shared_ptr<Obj::Object>
	help::invoke(std::vector<std::shared_ptr<Obj::Object>> args)
	{
		check_nargs(args, 0);
		return std::make_shared<Obj::String>(
            "say exit fopen tostr ascii len read fname is_open put"
        );
	}
}
