#include "expr.h"
#include "object.h"

namespace Ast {

    Ternary::Ternary(std::unique_ptr<Expr> cond, Tok op, std::unique_ptr<Expr> iftrue,
        std::unique_ptr<Expr> iffalse)
        : cond_(std::move(cond)), op_(op), iftrue_(std::move(iftrue)),
            iffalse_(std::move(iffalse)) {};

    std::shared_ptr<Obj::Object>
    Ternary::eval()
    {
        // cond is deleted here
        auto cond = cond_->eval();

        switch (op_.type()) {
            case Token::QUESTION: return cond->is_truthy() ? iftrue_->eval() : iffalse_->eval();
            default:
                throw std::runtime_error("Ternary: Unimplemented operator.");
        }
    }

    std::string
    Ternary::to_str() const
    {
        return cond_->to_str() + " " + op_.to_str() + " " + iftrue_->to_str() + " : " + iffalse_->to_str();
    }

}
