#ifndef INTPR_H
#define INTPR_H
#include "stmt.h"
#include "sym.h"
#include <stack>
#include <vector>

// Interpreter
class Intpr {
    std::vector<Stmt *> stmts;
    std::stack<Eval> stack;
    SymTable sym_table;
    
    public:
    Intpr(std::vector<Stmt *> stmts)
        : stmts(stmts) {};

    void interpret();
    static void error(Expr const *curr, std::string const &msg);
    static bool stob(std::string s);
};

#endif

