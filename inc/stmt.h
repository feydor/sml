#ifndef STMT_H
#define STMT_H
#include <vector>
#include "expr.h"

enum StmtType {
    EXPR, SAY,
}

class Stmt {
    StmtType type;
};

class Expr_stmt : Stmt {
    std::string name;
    Expr expr;

    Expr_stmt() {
        this.type = EXPR;
    }
};

class Say : Stmt {
    std::string name;
    std::string val;

    Expr_stmt() {
        this.type = SAY;
    }
};

#endif
