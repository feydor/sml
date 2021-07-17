#include "expr.h"

namespace Ast {
    std::shared_ptr<Obj::Object>
    Literal::eval()
    {
        return val_;
    }

    std::string
    Literal::to_str() const
    {
        return val_->to_str();
    }
}
