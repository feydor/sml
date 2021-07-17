#ifndef SMOL_LIBFILE_H
#define SMOL_LIBFILE_H
#include "ffi.h"
#include "object.h"

namespace Lib {
	namespace File {
		// method: is_open
		// prototype: is_open() -> bool
		class is_open : public FFInterface {
			public:
			std::string name() override;
			std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
		};

		// method: name
		// prototype: fname() -> string
		class fname : public FFInterface {
			public:
			std::string name() override;
			std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
		};

		// method: read
		// prototype: read() -> string
		class read : public FFInterface {
			public:
			std::string name() override;
			std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
		};
	}
}

#endif
