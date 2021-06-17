#include "expr.h"

namespace Ast {
std::string
Grouping::to_str() const
{
	return left()->to_str();
}
}