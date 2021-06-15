#include <iostream>
#include "intpr.h"
#include "value.h"

namespace Intpr {
// eval statements
// resolve variable typing
void
intpr::interpret(std::vector<Ast::Stmt *> stmts)
{
    Val::Val res;
    for (auto &_stmt : stmts) {
        if (_stmt->is_def_stmt()) {
            // var_decl with definition, add to symtable
            // if definition holds other undeclared vars/idents
            // error in eval
            auto stmt = (Ast::IdentStmt *)_stmt;
            eval_ast(stmt->expr());
            res = this->stack.top();

            auto def = new Var(stmt->ident()->sym(), res);
            envs.back().insert_var(def);

        } else if (_stmt->is_decl_stmt()) {
            // var_decl without definition, add to symtable as NIL value
            auto stmt = (Ast::IdentStmt *)_stmt;
            auto decl = new Var(stmt->ident()->sym());

            envs.back().insert_var(decl);

        } else if (_stmt->is_redef_stmt()) {
            auto stmt = (Ast::IdentStmt *)_stmt; 

            // search for var in envs; redefine once and break
            auto itr = envs.rbegin();
            for (; itr != envs.rend(); ++itr) {
                if ((*itr).in_env(stmt->ident()->sym())) {
                    eval_ast(stmt->expr());
                    res = this->stack.top();

                    auto redef = new Var(stmt->ident()->sym(), res);
                    (*itr).replace_var(redef);
                    break;
                }
            }

            if (itr == envs.rend()) {
                /* reached if var is undeclared  */
                intpr::error(stmt->expr(),
                        "Redefinition of un-declared variable.");
            }

        } else if (_stmt->is_block_stmt()) {
            // create new Env, add vars to it, interpret the block's stmts
            auto block = (Ast::BlockStmt *)_stmt;
            this->envs.emplace_back();
            interpret(block->get_stmts());
            this->envs.pop_back();

        } else {
            // do eval on stmt->expr, do not cout the result
            // can error if an identifier is not in symtable
            auto stmt = (Ast::ExprStmt *)_stmt;
            eval_ast(stmt->expr());

            // cout the result
            if (stmt->is_say_stmt())
                std::cout << "result: " <<
                    this->stack.top().to_string() << std::endl;
        }       
    }
}

/**
 * depth-first traversal eval
 * resolve user-defined identifiers
 */
void
intpr::eval_ast(Ast::Expr const *curr)
{
    if (curr->left()) eval_ast(curr->left());
    if (curr->right()) eval_ast(curr->right());
    Val::Val a, b, res;

    if (curr->is_ident()) {
        auto ident = (Ast::Ident *)curr;

        // look up in envs and resolve
        this->stack.push(resolve_sym(ident->sym()));

    } else if (curr->is_literal()) {
        auto literal = (Ast::Literal *) curr;
        // push onto stack
        this->stack.push(literal->val());

    } else if (curr->is_binary()) {
        // pop two off stack, evaluate, and push res
        auto binary = (Ast::Binary *) curr;
        b = this->stack.top();
        this->stack.pop();
        a = this->stack.top();
        this->stack.pop();
        
        stack.push(Ast::Binary::eval(a, binary->op(), b)); 

    } else if (curr->is_unary()) {
        // pop one off stack, eval, push back onto stack
        auto unary = (Ast::Unary *) curr;
        a = this->stack.top();
        this->stack.pop();

        stack.push(Ast::Unary::eval(unary->op(), a));

    } else if (curr->is_grouping()) {
        // do nothing; continue eval
    } else {
        intpr::error(curr, "Current expr type is not implemented in eval_ast.");
    }
}

// public interface to private interpret
void
intpr::interpret()
{
    interpret(this->stmts);
}

// populates curr with val and type from sym in sym_table
// Assumes curr.val_str is a key to a symbol in intpr::sym_table
// Assumes the symbol exists in the table and is valid
void
intpr::swap_with_sym(Val::Val &curr, Sym &sym)
{
    // TODO: Assuming no Fn, only Var
    Var *var = (Var *) &sym;
    curr = var->val();
    if (var->val().is_nil())
        std::cout << "Expr::swap_with_sym: Val is nil.\n";        
}

Val::Val
intpr::resolve_sym(Sym const &sym)
{
    // TODO: Assuming only Var
    Val::Val res;
    for (auto ritr = envs.rbegin(); ritr != envs.rend(); ++ritr) {
        if ((*ritr).in_env(sym)) {
            auto curr_env = *ritr;
            res = (((Var *)curr_env.get(sym))->val());
            break;
        }
    }
    return res;
}

void
intpr::sym_undefined_exit(Ast::Expr const *curr)
{
    intpr::error(curr, "Symbol not defined at eval-time.");
    exit(-1);
}

void
intpr::error(Ast::Expr const *_curr, std::string const &msg)
{
    if (_curr->is_binary()) {
        auto curr = (Ast::Binary *) _curr;
        std::cout << "intpr::error [line" << curr->op().get_line() <<
            "} Error at expression '" <<  "' " + msg + "]\n";
    } else { 
        auto curr = (Ast::Literal *) _curr;
        std::cout << "intpr::error  Error " <<
            "at expression '" << curr->val().to_string() << "' " + msg << std::endl;
    }
}

// string to bool
// input should be lowercase "true" or "false"
bool
intpr::stob(std::string s)
{
    return s.compare("true") == 0 ? true : false;
}
}
