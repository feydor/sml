#include "stmt.h"
#include "expr.h"
#include "env.h"
#include <iostream>

namespace Ast {

    void
    BlockStmt::exec()
    {
        for (auto& stmt : stmts_)
            stmt->exec();
    }

    BlockStmt::~BlockStmt()
    {
        for (auto& stmt : stmts_)
            delete stmt;
    }

    void
    BlockStmt::add_stmt(Stmt *stmt)
    {
        stmts_.push_back(stmt);
    }

}
