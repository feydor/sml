#include "expr.h"

namespace Ast {
    std::shared_ptr<Obj::Object>
    MethodExpr::eval()
    {
        // eval the args and push into array,
        // then, either
        // 1. turn into a string like name;arg1;arg2;arg3... or
        // 2. use Object::Array like [name, arg1, arg2, arg3, ...]
        std::vector<std::shared_ptr<Obj::Object>> methodcall = {};
        methodcall.push_back(std::make_shared<Obj::String>(name_));
        for (const auto& arg : args_)
            methodcall.push_back(arg->eval());
        return std::make_unique<Obj::Array>(methodcall);
    }

    void
    MethodExpr::add_arg(Expr* arg)
    {
        args_.push_back(arg);
    }

    std::string
    MethodExpr::to_str() const
    {
        std::string res(name_ + " (");
        for (const auto& arg : args_)
            res += arg->to_str() + ", ";
        return res.substr(0, res.size() - 2) + ")";
    }

    MethodExpr::~MethodExpr()
    {
        for (auto& arg : args_)
            delete arg;
    }
}
