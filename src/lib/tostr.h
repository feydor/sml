#ifndef SMOL_TOSTR_H
#define SMOL_TOSTR_H
#include "fn.h"

namespace Lib {
	class to_str : public Fn {
		public:
		std::string name() override;
		Val invoke(std::vector<Val> args) override;
	};
}

#endif