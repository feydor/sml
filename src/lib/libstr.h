#ifndef SMOL_LIBSTR_H
#define SMOL_LIBSTR_H
#include "ffi.h"

namespace Lib {
	namespace String {
		// method: size
		// prototype: size() -> number
		class size : public FFInterface {
			public:
			std::string name() override;
			std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
		};

		std::string
		size::name()
		{
			return "size";
		}

		// the first args should be a pointer to this object
		std::shared_ptr<Obj::Object>
		size::invoke(std::vector<std::shared_ptr<Obj::Object>> args)
		{
			check_nargs(args, 1);
			auto this_str = std::dynamic_pointer_cast<Obj::String>(args[0]);
			return std::make_shared<Obj::Number>(this_str->str().size());
		}

		// method: at
		// prototype: at(number i) -> 'character'/string
		class at : public FFInterface {
			public:
			std::string name() override;
			std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
		};

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

#endif
