#include "expr.h"
#include "intpr.h"
#include "parser.h"
#include "smol.h"
#include "sym.h"
#include <iostream>
#include <cstring>
#include <string>

namespace Ast {

/**
 * depth-first traversal eval
 * resolve user-defined identifiers
 */
void
Expr::eval_ast(Expr const *curr, std::stack<Eval> &stack, SymTable const &sym_table)
{
    if (curr->left) eval(curr->left, stack, sym_table);
    if (curr->right) eval(curr->right, stack, sym_table);
    Val::Val a, b, res;
    switch (curr->type) {
        case Expr_t::IDENT:

        case Expr_t::LITERAL:
            // possible val_types are:
            // IDENT, NUMBER, STRING, BOOL
            res.val_type = curr->val_type;
            res.val = curr->val;

            // look up user-defined identifiers in sym_table
            // must be defined by this point
            // converts to either NUMBER, STRING, or BOOL
            if (res.val_type == Val::IDENT) {
                if (!sym_table.in_table(res.val))
                    sym_undefined_exit(curr);
                swap_with_sym(res, sym_table);
            }
            stack.push(res);
            break;
        case Expr_t::BINARY:
            // pop two off stack, evaluate, and push res
            b = stack.top();
            stack.pop();
            a = stack.top();
            stack.pop();

            // std::cout << a.val << " : " << b.val << std::endl;
            // std::cout << a.type_to_string() << " : " <<
            //    b.type_to_string() << std::endl;

            // at this point, both a and be need to have non-IDENT Val
            // and both must be the same
            res.val_type = a.val_type;
            if (a.val_type != b.val_type) {
                Intpr::error(curr, "Literal val_types do not match.");
                res.val = "ERR";
                res.val_type = Val::NIL;
                stack.push(res);
                return;
            }

            // eval number or string
            // TODO: binary operator of two bools, ops, nils
            if (a.val_type == Val::NUM)
                res.val = eval_binary(curr->op, 
                        std::stod(a.val), std::stod(b.val));
            else
                res.val = eval_binary(curr->op, a.val, b.val);

            stack.push(res);
            break;
        case Expr_t::GROUPING:
            // do nothing here
            break;
        case Expr_t::UNARY:
            // pop one off stack, modify with unary, push back onto stack
            a = stack.top();
            stack.pop();

            if (a.val_type == Val::IDENT) {
                if (!sym_table.in_table(a.val))
                    sym_undefined_exit(curr);
                swap_with_sym(a, sym_table);
            }

            // TODO: what does unary do to strings, groups, bools, ops, nil
            // use switch statement for valid options, emit error on inval
            if (a.val_type == Val::NUM)
                a.val = eval_unary(curr->op, std::stod(a.val));
            else if (a.val_type == Val::GROUP)
                a.val = eval_unary(curr->op, std::stod(a.val));
            stack.push(a);
            break;
        case Expr_t::KEYWORD:
            // do nothing here
            break;
    }
    return;
}

std::string
Expr::eval_unary(Token const &tok, double a)
{
    double res = 0;
    switch (tok.type) {
        case MINUS: res = -a; break;
        default: SMOL::error(tok, "eval_unary: Unexpected operator on literal.");
    }
    return std::to_string(res);
}

// populates curr with val and type from sym in sym_table
// Assumes curr.val_str is a key to a symbol in Intpr::sym_table
// Assumes the symbol exists in the table and is valid
void
Expr::swap_with_sym(Val::Val &curr, SymTable const &sym_table)
{
    auto var = (Var *)sym_table.get(curr.val_str);
    if (var->val.is_num() || var->val.is_str() || var->val.is_bool())
        curr.val = var->val;
    if (var->val.is_nil())
        std::cout << "Expr::swap_with_sym: Val is nil.\n";        
}

void
Expr::sym_undefined_exit(Expr const *curr)
{
    Intpr::error(curr, "Symbol not defined at eval-time.");
    exit(-1);
}

std::string
Expr::to_string(bool b)
{
    return b ? "true" : "false";
}

}

