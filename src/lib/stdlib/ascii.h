#ifndef SMOL_ASCII_H
#define SMOL_ASCII_H
#include "ffi.h"
#include "object.h"

namespace Lib {
	class ascii : public FFInterface {
		public:
		std::string name() override;
		std::shared_ptr<Obj::Object> invoke(std::vector<std::shared_ptr<Obj::Object>> args) override;
		private:
		std::string num_to_ascii(double num);
	};
}

#endif
