#include "stmt.h"
#include "expr.h"
#include "object.h"
#include <iostream>

namespace Ast {
	SayStmt::SayStmt(std::unique_ptr<Ast::Expr> expr)
        : expr_(std::move(expr)) {}

	void
	SayStmt::exec()
	{
		if (expr_ == nullptr) {
			std::cout << "<fn say>\n";
			return;
		}

		auto out = expr_->eval()->to_str();
		for (unsigned i = 0; i < out.size(); ++i) {
			if (out[i] == '\\') {
				i++;
				// switch on special character ex: '\n'
				switch (out[i]) {
					case 'n': std::cout << "\n"; break;
				}
			} else {
				std::cout << out[i];
			}
		}
		std::cout << std::endl;
	}
}
