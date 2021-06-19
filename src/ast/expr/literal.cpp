#include "expr.h"

namespace Ast {
    Val
    Literal::eval()
    {
        return val_;
    }

    Literal::~Literal() {};

    std::string
    Literal::to_str() const
    {
        return val_.to_str();
    }
}
