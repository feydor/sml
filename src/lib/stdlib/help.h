#ifndef SMOL_HELP_H
#define SMOL_HELP_H
#include "ffi.h"
#include "object.h"

namespace Lib {
	class help : public FFInterface {
		public:
		std::string name() override;
		std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
	};
}

#endif
