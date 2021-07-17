#include "fopen.h"

namespace Lib {
	std::string
	fopen::name()
	{
		return "fopen";
	}

	std::shared_ptr<Obj::Object>
	fopen::invoke(std::vector<std::shared_ptr<Obj::Object>> args)
	{
		check_nargs(args, 2);
		return std::make_shared<Obj::File>(args[0]->to_str(), args[1]->to_str());
	}
}
