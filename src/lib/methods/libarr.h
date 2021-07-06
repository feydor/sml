#ifndef SMOL_LIBARR_H
#define SMOL_LIBARR_H
#include "ffi.h"

namespace Lib {
	namespace Array {
		// method: put
		// prototype: put(index, Object) -> void
		class put : public FFInterface {
			public:
			std::string name() override;
			std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
		};
	}
}

#endif
