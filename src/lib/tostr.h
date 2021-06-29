#ifndef SMOL_TOSTR_H
#define SMOL_TOSTR_H
#include "ffi.h"

namespace Lib {
	class to_str : public FFInterface {
		public:
		std::string name() override;
		Obj::Object* invoke(std::vector<Obj::Object*> args) override;
	};
}

#endif
