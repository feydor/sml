#include "expr.h"
#include <iostream>

namespace Ast {

    Val
    Ternary::eval()
    {
        Val cond = cond_->eval();

        switch (op_.type()) {
            case Token::QUESTION: return cond.is_truthy() ? iftrue_->eval() : iffalse_->eval();
            default:
                std::cout << "Ternary:eval: Unimplemented operator.\n";
                return Val(); // nil
        }
    }

    Ternary::~Ternary()
    {
        delete cond_;
        delete iftrue_;
        delete iffalse_;
    }

    std::string
    Ternary::to_str() const
    {
        return cond_->to_str() + " " + op_.to_str() + " " + iftrue_->to_str() + " : " + iffalse_->to_str();
    }

}
