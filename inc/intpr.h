#ifndef INTPR_H
#define Intpr_H
#include <vector>
#include "stmt.h"

// Interpreter
class Intpr {
    std::vector<Stmt *> stmts;
    
    public:
    Intpr(std::vector<Stmt *> stmts)
        : stmts(stmts) {};

    void interpret();
};

#endif

