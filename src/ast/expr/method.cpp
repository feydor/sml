#include "expr.h"
#include "object.h"

namespace Ast {
    MethodExpr::MethodExpr(std::string name) : name_(name) {};

    std::shared_ptr<Obj::Object>
    MethodExpr::eval()
    {
        // eval the args and push into array,
        // then use Object::Array like [name, arg1, arg2, arg3, ...]
        std::vector<std::shared_ptr<Obj::Object>> methodcall = {};
        methodcall.push_back(std::make_shared<Obj::String>(name_));
        for (const auto& arg : args_)
            methodcall.push_back(arg->eval());
        return std::make_shared<Obj::Array>(methodcall);
    }

    void
    MethodExpr::add_arg(std::unique_ptr<Expr> arg)
    {
        args_.push_back(std::move(arg));
    }

    std::string
    MethodExpr::to_str() const
    {
        std::string res(name_ + " (");
        for (const auto& arg : args_)
            res += arg->to_str() + ", ";
        return res.substr(0, res.size() - 2) + ")";
    }
}
