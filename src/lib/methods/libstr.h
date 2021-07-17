#ifndef SMOL_LIBSTR_H
#define SMOL_LIBSTR_H
#include "ffi.h"
#include "object.h"

namespace Lib {
	namespace String {
		// method: len
		// prototype: len() -> number
		class len : public FFInterface {
			public:
			std::string name() override;
			std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
		};

		// method: at
		// prototype: at(number i) -> 'character'/string
		class at : public FFInterface {
			public:
			std::string name() override;
			std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
		};
	}
}

#endif
