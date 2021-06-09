#ifndef INTPR_H
#define Intpr_H
#include <stack>
#include <vector>
#include "stmt.h"

// Interpreter
class Intpr {
    std::vector<Stmt *> stmts;
    std::stack<Eval> stack;
    
    public:
    Intpr(std::vector<Stmt *> stmts)
        : stmts(stmts) {};

    void interpret();
};

#endif

