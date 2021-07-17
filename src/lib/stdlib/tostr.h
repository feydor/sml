#ifndef SMOL_TOSTR_H
#define SMOL_TOSTR_H
#include "ffi.h"
#include "object.h"

namespace Lib {
	class to_str : public FFInterface {
		public:
		std::string name() override;
		std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
	};
}

#endif
