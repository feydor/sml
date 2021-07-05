#ifndef SMOL_LIBSTR_H
#define SMOL_LIBSTR_H
#include "ffi.h"

namespace Lib {
	namespace String {
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
			auto this_str = std::dynamic_pointer_cast<Obj::String>(args[0]);
			return std::make_shared<Obj::Number>(this_str->size());
		}
	}
}

#endif
