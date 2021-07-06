#include "libfile.h"

namespace Lib {
	namespace File {
		std::string
		is_open::name()
		{
			return "is_open";
		}

		std::shared_ptr<Obj::Object>
		is_open::invoke(std::vector<std::shared_ptr<Obj::Object>> args)
		{
			check_nargs(args, 1);
			auto this_file = std::dynamic_pointer_cast<Obj::File>(args[0]);
			return std::make_unique<Obj::Bool>(this_file->is_open());
		}

		std::string
		fname::name()
		{
			return "fname";
		}

		std::shared_ptr<Obj::Object>
		fname::invoke(std::vector<std::shared_ptr<Obj::Object>> args)
		{
			check_nargs(args, 1);
			auto this_file = std::dynamic_pointer_cast<Obj::File>(args[0]);
			return std::make_unique<Obj::String>(this_file->filename());
		}

		std::string
		read::name()
		{
			return "read";
		}

		std::shared_ptr<Obj::Object>
		read::invoke(std::vector<std::shared_ptr<Obj::Object>> args)
		{
			check_nargs(args, 1);
			auto this_file = std::dynamic_pointer_cast<Obj::File>(args[0]);
			return std::make_unique<Obj::String>(this_file->read());
		}
	}
}
