#ifndef AST_H
#define AST_H
#include <vector>
#include "stmt.h"

struct AST {
    std::vector<Stmt *> stmts;
};

#endif
