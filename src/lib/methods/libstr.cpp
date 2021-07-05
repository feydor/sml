#include "libstr.h"

namespace Lib {
	namespace String {
		std::string
		len::name()
		{
			return "len";
		}

		std::shared_ptr<Obj::Object>
		len::invoke(std::vector<std::shared_ptr<Obj::Object>> args)
		{
			check_nargs(args, 1);
			auto this_str = std::dynamic_pointer_cast<Obj::String>(args[0]);
			return std::make_shared<Obj::Number>(this_str->str().size());
		}

		std::string
		at::name()
		{
			return "at";
		}

		std::shared_ptr<Obj::Object>
		at::invoke(std::vector<std::shared_ptr<Obj::Object>> args)
		{
			check_nargs(args, 2);
			auto this_str = std::dynamic_pointer_cast<Obj::String>(args[0]);
			auto index = std::dynamic_pointer_cast<Obj::Number>(args[1])->num();
			std::string char_to_str(1, this_str->str()[index]);
			return std::make_shared<Obj::String>(char_to_str);
		}
	}
}
