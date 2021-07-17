#include "stmt.h"
#include "expr.h"
#include "env.h"
#include <iostream>

namespace Ast {
    WhileStmt::WhileStmt(std::unique_ptr<Ast::Expr> cond, std::unique_ptr<Ast::Stmt> body)
        : cond_(std::move(cond)), body_(std::move(body)) {};

    void
    WhileStmt::exec()
    {
        Env::push_back();
        while (cond_->eval()->is_truthy())
            body_->exec();
        Env::pop();
    }

    WhileStmt::~WhileStmt()
    {
        // delete cond_;
        // delete body_;
    }

}
