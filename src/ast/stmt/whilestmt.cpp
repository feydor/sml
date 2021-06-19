#include "stmt.h"
#include "expr.h"
#include "env.h"
#include <iostream>

namespace Ast {

    void
    WhileStmt::exec()
    {
        Env::push_back();
        while (cond_->eval().is_truthy())
            body_->exec();
        Env::pop();
    }

    WhileStmt::~WhileStmt()
    {
        delete cond_;
        delete body_;
    }

}
