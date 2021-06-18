#include "expr.h"
#include "symtable.h"

namespace Ast {

    Val
    Var::eval()
    {
        return SymTable::get_var(name_);
    }

    std::string
    Var::to_str() const
    {
        return name_;
    }

}
