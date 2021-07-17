#include "expr.h"
#include "object.h"

namespace Ast {
    Literal::Literal(std::unique_ptr<Obj::Object> val)
        : val_(std::move(val)) {};

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
