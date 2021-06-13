#include <iostream>
#include "intpr.h"
#include "value.h"

// eval statements
// resolve variable typing
void
Intpr::interpret()
{
    Val::Val res;
    for (auto &_stmt : this->stmts) {
        if (_stmt->is_def_stmt()) {
            auto stmt = (Ast::IdentStmt *)_stmt;
            // var_decl with definition, add to symtable
            /* assuming expr does not have other vars/identifiers
             * TODO: in expr->eval, deal with undefined vars/idents
             */
            eval_ast(stmt->expr());
            res = this->stack.top();

            if (res.is_num()) {
                auto vdef = new Var(stmt->ident()->sym().sym,
                        res.get_num());
                this->sym_table.insert(vdef);
            } else if (res.is_str()) {
                auto vdef = new Var(stmt->ident()->sym().sym,
                        res.get_str());
                this->sym_table.insert(vdef);
            } else if (res.is_bool()) {
                auto vdef = new Var(stmt->ident()->sym().sym,
                        res.get_bool());
                this->sym_table.insert(vdef);
            } else {
                Intpr::error(stmt->ident(),
                        "Variable definition is not string, number, or bool.");
                break;
            }
        } else if (_stmt->is_decl_stmt()) {
            auto stmt = (Ast::IdentStmt *)_stmt;
            // var_decl without definition, add to symtable?
            // TODO: add to sym_table with value of NIL
            std::cout << "VAR_DECL without definition is not"
                << "implemented yet." << std::endl;
        } else {
            auto stmt = (Ast::ExprStmt *)_stmt;
            // do eval on stmt->expr, do not cout the result
            // can error if an identifier is not in symtable
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
Intpr::eval_ast(Ast::Expr const *curr)
{
    if (curr->left()) eval_ast(curr->left());
    if (curr->right()) eval_ast(curr->right());
    Val::Val a, b, res;

    if (curr->is_ident()) {
        auto ident = (Ast::Ident *)curr;
        // look up in sym_table and resolve
        if (!sym_table.in_table(ident->sym()))
            sym_undefined_exit(ident);
        res = ((Var *)sym_table.get(ident->sym()))->val;
        this->stack.push(res);

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

        if (!a.same_type(b)) {
            Intpr::error(curr, "Literal val types do not match.");
            Val::Val nil = Val::Val();
            stack.push(nil);
            return;
        }
        
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
        Intpr::error(curr, "Current expr type is not implemented in eval_ast.");
    }
}

// populates curr with val and type from sym in sym_table
// Assumes curr.val_str is a key to a symbol in Intpr::sym_table
// Assumes the symbol exists in the table and is valid
void
Intpr::swap_with_sym(Val::Val &curr, Sym &sym)
{
    // TODO: Assuming no Fn, only Var
    Var *var = (Var *) &sym;
    curr = var->val;
    if (var->val.is_nil())
        std::cout << "Expr::swap_with_sym: Val is nil.\n";        
}

void
Intpr::sym_undefined_exit(Ast::Expr const *curr)
{
    Intpr::error(curr, "Symbol not defined at eval-time.");
    exit(-1);
}

void
Intpr::error(Ast::Expr const *_curr, std::string const &msg)
{
    if (_curr->is_binary()) {
        auto curr = (Ast::Binary *) _curr;
        std::cout << "Intpr::error [line" << curr->op().line <<
            "} Error at expression '" <<  "' " + msg + "]\n";
    } else { 
        auto curr = (Ast::Literal *) _curr;
        std::cout << "Intpr::error  Error " <<
            "at expression '" << curr->val().to_string() << "' " + msg << std::endl;
    }
}

// string to bool
// input should be lowercase "true" or "false"
bool
Intpr::stob(std::string s)
{
    return s.compare("true") == 0 ? true : false;
}
