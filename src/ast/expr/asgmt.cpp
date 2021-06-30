#include "expr.h"

namespace Ast {

    std::shared_ptr<Obj::Object>
    Asgmt::eval()
    {
        return right_->eval();
    }

    Asgmt::~Asgmt()
    {
        delete left_;
        delete right_;
    }

    std::string
    Asgmt::to_str() const
    {
        return left_->to_str() + " = " + right_->to_str();
    }

}
