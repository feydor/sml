#include "ascii.h"
#include <ostream>

namespace Lib {
	std::string
	ascii::name()
	{
		return "ascii";
	}

	std::shared_ptr<Obj::Object>
	ascii::invoke(std::vector<std::shared_ptr<Obj::Object>> args)
	{
		check_nargs(args, 1);
		auto num = std::dynamic_pointer_cast<Obj::Number>(args[0])->num();
		std::string char_to_string(1, num);
		return std::make_unique<Obj::String>(char_to_string);
	}

	std::string
	ascii::num_to_ascii(double num)
	{

		return std::to_string('0' + num);
		// std::ostringstream oss;
		// oss << num;
		// return oss.str();
	}
}
