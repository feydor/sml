#include "stmt.h"
#include "env.h"
#include "expr.h"
#include "object.h"

namespace Ast {

    void
    BlockStmt::exec()
    {
        for (const auto& stmt : stmts_) {
            stmt->exec();
        }
    }

    void
    BlockStmt::add_stmt(std::unique_ptr<Stmt> stmt)
    {
        stmts_.push_back(std::move(stmt));
    }

}
