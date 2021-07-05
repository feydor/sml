#ifndef SMOL_EXIT_H
#define SMOL_EXIT_H
#include "ffi.h"

namespace Lib {
	class smol_exit : public FFInterface {
		public:
		std::string name() override;
		std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
	};
}

#endif
