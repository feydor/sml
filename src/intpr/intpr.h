#ifndef INTPR_H
#define INTPR_H
#include "stmt.h"
#include "sym.h"
#include "value.h"
#include <stack>
#include <vector>

// Interpreter
class Intpr {
    std::vector<Ast::Stmt *> stmts;
    std::stack<Val::Val> stack;
    SymTable sym_table;
    
    public:
    Intpr(std::vector<Ast::Stmt *> stmts)
        : stmts(stmts) {};

    void interpret();
    static void error(Ast::Expr const *curr, std::string const &msg);
    static bool stob(std::string s);
};

#endif

