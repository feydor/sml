#include <iostream>
#include "intpr.h"
#include "sym.h"

void
Intpr::interpret()
{
    std::cout << "\n begin interpretation...\n";
    std::string res;
    SymTable sym_table;
    /*
    for (auto &stmt : this->stmts) {
        // TODO: Move this into stmt.cpp,
        // called as stmt->eval(this->stack)
        // which internally calls eval on it's expr
        if (stmt->type == VAR_DECL) {
            if (!stmt->expr) {
                // var_decl without definition, add to symtable
            }
            // var_decl with definition, add to symtable
        } else if (stmt->type == SAY_STMT) {
           // do eval on stmt.expr and then std::cout the result  
           // can error if an identifier is not in symtable
        } else if (stmt->type == EXPR_STMT) {
            // do eval on stmt->expr, do not cout the result
        }
        
        // root->eval(root, this->stack);
        // root->print_tree(root, res);
        std::cout << res << std::endl;
        std::cout << "result: " << this->stack.top().val << std::endl;
    }
    */
}
