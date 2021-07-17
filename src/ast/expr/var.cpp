#include "expr.h"
#include "env.h"
#include "object.h"

namespace Ast {
    Var::Var(std::string name) : name_(name) {};

    std::shared_ptr<Obj::Object>
    Var::eval()
    {
        return Env::get_var(name_);
    }

    std::string
    Var::to_str() const
    {
        return name_;
    }

}
