#ifndef INTPR_H
#define INTPR_H
#include "stmt.h"
#include "expr.h"
#include "env.h"
#include "value.h"
#include <stack>
#include <vector>

namespace Intpr {
class intpr {
    std::vector<Ast::Stmt *> stmts;
    std::stack<Val::Val> stack;
    std::vector<Env> envs;
    
    public:
    intpr(std::vector<Ast::Stmt *> stmts)
        : stmts(stmts) {
        // start with globals environment
        envs.emplace_back();
    }

    void interpret();

    private:
    void interpret_stmts(std::vector<Ast::Stmt *> stmts);
    void interpret_one(Ast::Stmt *_stmt);
    void eval_ast(Ast::Expr const *curr);
    void swap_with_sym(Val::Val &curr, Sym &sym);
    Val::Val resolve_sym(Sym const &sym);
    static void error(Ast::Expr const *curr, std::string const &msg);
    void sym_undefined_exit(Ast::Expr const *curr);
    static bool stob(std::string s);
};
}

#endif

