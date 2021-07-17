#include "expr.h"
#include <iostream>

namespace Ast {

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

    Ternary::~Ternary()
    {
        /*
        delete cond_;
        delete iftrue_;
        delete iffalse_;
        */
    }

    std::string
    Ternary::to_str() const
    {
        return cond_->to_str() + " " + op_.to_str() + " " + iftrue_->to_str() + " : " + iffalse_->to_str();
    }

}
