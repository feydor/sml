#include "expr.h"
#include "vartable.h"

namespace Ast {

    Val
    Var::eval()
    {
        return VarTable::get_var(name_);
    }

    std::string
    Var::to_str() const
    {
        return name_;
    }

}
