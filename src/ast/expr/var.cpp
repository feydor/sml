#include "expr.h"
#include "env.h"

namespace Ast {

    Val
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
