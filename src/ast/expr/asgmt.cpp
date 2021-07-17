#include "expr.h"
#include "object.h"

namespace Ast {
    Asgmt::Asgmt(std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
        : left_(std::move(left)), right_(std::move(right)) {};

    std::shared_ptr<Obj::Object>
    Asgmt::eval()
    {
        return right_->eval();
    }

    std::string
    Asgmt::to_str() const
    {
        return left_->to_str() + " = " + right_->to_str();
    }

}
