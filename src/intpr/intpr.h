#ifndef INTPR_H
#define INTPR_H
#include "stmt.h"
#include "expr.h"
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
    void eval_ast(Ast::Expr const *curr);
    void swap_with_sym(Val::Val &curr, Sym &sym);
    static void error(Ast::Expr const *curr, std::string const &msg);
    void sym_undefined_exit(Ast::Expr const *curr);
    static bool stob(std::string s);
};

#endif

