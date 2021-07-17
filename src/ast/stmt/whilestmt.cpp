#include "stmt.h"
#include "env.h"
#include "expr.h"
#include "object.h"

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

}
