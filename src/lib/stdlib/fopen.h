#ifndef SMOL_FOPEN_H
#define SMOL_FOPEN_H
#include "ffi.h"

namespace Lib {
	class fopen : public FFInterface {
		public:
		std::string name() override;
		std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
	};
}

#endif
