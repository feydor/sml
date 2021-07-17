#include "expr.h"
#include "object.h"
#include <iostream>

namespace Ast {
    Unary::Unary(Tok op, std::unique_ptr<Expr> right)
        : op_(op), right_(std::move(right)) {};

    std::shared_ptr<Obj::Object>
    Unary::eval()
    {
        // right is deleted here
        auto x = right_->eval();

        switch (op_.type()) {

            case Token::MINUS: return -*x;
            case Token::BANG: return !*x;
            case Token::PLUS_PLUS: return (*x)++;
            case Token::MINUS_MINUS: return (*x)--;
            default:
                throw std::runtime_error("Unary: Unexpected operator. " + op_.to_str());
        }
        /* not reached */
        return nullptr;
    }

    std::string
    Unary::to_str() const
    {
        return op_.to_str() + right_->to_str();
    }
}
