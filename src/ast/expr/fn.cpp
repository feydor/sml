#include "expr.h"
#include "functable.h"

namespace Ast {

    // lookup fn in sym table and invoke with args evaluated into values
    Val
    eval()
    {
        std::vector<Val> args;
        for (auto& expr : exprs_)
            args.push_back(expr->eval());
        return FuncTable::get_fn(name_)->invoke(args);
    }

    Fn::~Fn()
    {
        for (auto& expr : exprs_)
            delete expr;
    }

    std::string
    to_str() const
    {
        std::string res("");
        for (auto& expr : exprs_)
            res += expr->to_str() + " ";
        return res;
    }

    void
    add_arg(Expr* expr)
    {
        exprs_.push_back(expr);
    }

}
