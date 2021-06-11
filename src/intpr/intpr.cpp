#include <iostream>
#include "intpr.h"

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
        if (stmt->type == VAR_DECL) {
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
            switch (res.val_type) {
                case Val_t::NUM:
                    {
                        auto vdef = new Var(stmt->ident->val,
                                std::stod(res.val));
                        sym_table.insert(vdef);
                    }
                    break;
                case Val_t::STR:
                    {
                        auto vdef = new Var(stmt->ident->val, res.val);
                        sym_table.insert(vdef);
                    }
                    break;
                case Val_t::BOOL:
                    {
                        auto vdef = new Var(stmt->ident->val,
                                Intpr::stob(res.val));
                        sym_table.insert(vdef);
                    }
                    break;
                default:
                    Intpr::error(stmt->ident,
                            "Variable definition is not string, number, or bool.");
                    break;
            }
        } else {
            // do eval on stmt->expr, do not cout the result
            // can error if an identifier is not in symtable
            stmt->expr->eval(stmt->expr, this->stack, this->sym_table);

            // cout the result
            if (stmt->type == SAY_STMT)
                std::cout << "result: " << this->stack.top().val << std::endl;
        }       
        // root->print_tree(root, res);
    }
}

void
Intpr::error(Expr const *curr, std::string const &msg)
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
