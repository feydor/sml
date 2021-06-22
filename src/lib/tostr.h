#ifndef SMOL_TOSTR_H
#define SMOL_TOSTR_H
#include "ffi.h"

namespace Lib {
	class to_str : public FFInterface {
		public:
		std::string name() override;
		Val invoke(std::vector<Val> args) override;
	};
}

#endif
