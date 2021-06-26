#include "stmt.h"
#include <iostream>

namespace Ast {
	void
	SayStmt::exec()
	{
		auto out = expr_->eval().to_str();
		for (unsigned i = 0; i < out.size(); ++i) {
			if (out[i] == '\\') {
				i++;
				switch (out[i]) {
					case 'n':
						std::cout << "\n";
						break;
				}
			} else {
				std::cout << out[i];
			}
		}
		std::cout << std::endl;
	}

	SayStmt::~SayStmt()
	{
		delete expr_;
	}
}