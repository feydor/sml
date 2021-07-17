#ifndef SMOL_GETCHAR_H
#define SMOL_GETCHAR_H
#include "ffi.h"
#include "object.h"

namespace Lib {
	class smol_getchar : public FFInterface {
		public:
		std::string name() override;
		std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
	};
}

#endif
