#include "libarr.h"
#include <iostream>

namespace Lib {
	namespace Array {
		std::string
		put::name()
		{
			return "put";
		}

		std::shared_ptr<Obj::Object>
		put::invoke(std::vector<std::shared_ptr<Obj::Object>> args)
		{
			check_nargs(args, 3);
			auto this_arr = std::dynamic_pointer_cast<Obj::Array>(args[0]);
			auto index = std::dynamic_pointer_cast<Obj::Number>(args[1])->num();
			this_arr->put(index, args[2]);
			return std::make_unique<Obj::Number>(0.0);
		}
	}
}
