#include <iostream>
#include "intpr.h"
#include "value.h"

// eval statements
// resolve variable typing
void
Intpr::interpret()
{
    Eval res;
    for (auto &stmt : this->stmts) {
        // TODO: Move this into stmt.cpp,
        // called as stmt->eval(this->stack)
        // which internally calls eval on it's expr
        if (stmt->type == Stmt_t::VAR_DECL) {
            if (!stmt->expr)
                // var_decl without definition, add to symtable?
                // TODO: add to sym_table with value of NIL
                std::cout << "VAR_DECL without definition is not"
                    << "implemented yet." << std::endl;

            // var_decl with definition, add to symtable
            /* assuming expr does not have other vars/identifiers
             * TODO: in expr->eval, deal with undefined vars/idents
             */
            stmt->expr->eval(stmt->expr, this->stack, this->sym_table);
            res = this->stack.top();
            if (res.is_num() || res.is_str() || res.is_bool()) {
                auto vdef = new Var(stmt->ident()->sym(), res);
                sym_table.insert(vdef);
            } else {
                Intpr::error(stmt->ident(),
                        "Variable definition is not string, number, or bool.");
                break;

            }
        } else {
            // do eval on stmt->expr, do not cout the result
            // can error if an identifier is not in symtable
            stmt->expr->eval(stmt->expr, this->stack, this->sym_table);

            // cout the result
            if (stmt->type == Stmt_t::SAY)
                std::cout << "result: " << this->stack.top().val << std::endl;
        }       
        // root->print_tree(root, res);
    }
}

void
Intpr::error(Ast::Expr const *curr, std::string const &msg)
{
    std::cout << "Intpr::error [line " << curr->op.line << "] Error " <<
        "at expression '" << curr->val << "' " + msg << std::endl;
}

// string to bool
// input should be lowercase "true" or "false"
bool
Intpr::stob(std::string s)
{
    return s.compare("true") == 0 ? true : false;
}
