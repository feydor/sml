#include "expr.h"
#include "fntable.h"
#include "object.h"

namespace Ast {
    FnExpr::FnExpr(std::string name) : name_(name) {};

    std::shared_ptr<Obj::Object>
    FnExpr::eval()
    {
        if (!FnTable::exists(name_))
            throw std::runtime_error("Function " + name_ + " is not a defined function.");

        std::vector<std::shared_ptr<Obj::Object>> args = {};
        for (const auto& arg : args_)
            args.push_back(arg->eval());

        return FnTable::get_fn(name_)->invoke(std::move(args));
    }

    void
    FnExpr::add_arg(std::unique_ptr<Expr> arg)
    {
        args_.push_back(std::move(arg));
    }

    std::string
    FnExpr::to_str() const
    {
        std::string res(name_ + " (");
        for (const auto& arg : args_)
            res += arg->to_str() + ", ";
        return res.substr(0, res.size() - 2) + ")";
    }
}
