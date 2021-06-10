#include "expr.h"
#include "intpr.h"
#include "parser.h"
#include "smol.h"
#include "sym.h"
#include <iostream>
#include <cstring>
#include <string>


/**
 * depth-first traversal eval
 * resolve user-defined identifiers
 */
void
Expr::eval(Expr const *curr, std::stack<Eval> &stack, SymTable const &sym_table)
{
    if (curr->left) eval(curr->left, stack, sym_table);
    if (curr->right) eval(curr->right, stack, sym_table);
    // std::cout << curr->op.lexeme << " : " << curr->val << std::endl;
    Eval a, b, res;
    switch (curr->type) {
        case LITERAL:
            // possible val_types are:
            // IDENT, NUMBER, STRING, BOOL
            res.val_type = curr->val_type;
            res.val = curr->val;

            // look up user-defined identifiers in sym_table
            // must be defined by this point
            // converts to either NUMBER, STRING, or BOOL
            if (res.val_type == Val_t::IDENT) {
                if (!sym_table.in_table(res.val))
                    sym_undefined_exit(curr);
                swap_with_sym(res, sym_table);
            }
            stack.push(res);
            break;
        case BINARY:
            // pop two off stack, evaluate, and push res
            b = stack.top();
            stack.pop();
            a = stack.top();
            stack.pop();

            // std::cout << a.val << " : " << b.val << std::endl;
            // std::cout << Expr::to_string(a.val_type) << " : " <<
            //    Expr::to_string(b.val_type) << std::endl;

            // at this point, both a and be need to have non-IDENT Val_t
            // and both must be the same
            res.val_type = a.val_type;
            if (a.val_type != b.val_type) {
                Intpr::error(curr, "Literal val_types do not match.");
                res.val = "ERR";
                res.val_type = Val_t::NIL;
                stack.push(res);
                return;
            }

            // eval number or string
            // TODO: binary operator of two bools, ops, nils
            if (a.val_type == Val_t::NUM)
                res.val = eval_binary(curr->op, 
                        std::stod(a.val), std::stod(b.val));
            else
                res.val = eval_binary(curr->op, a.val, b.val);

            stack.push(res);
            break;
        case GROUPING:
            // do nothing here
            break;
        case UNARY:
            // pop one off stack, modify with unary, push back onto stack
            a = stack.top();
            stack.pop();

            if (a.val_type == Val_t::IDENT) {
                if (!sym_table.in_table(a.val))
                    sym_undefined_exit(curr);
                swap_with_sym(a, sym_table);
            }

            // TODO: what does unary do to strings, groups, bools, ops, nil
            // use switch statement for valid options, emit error on inval
            if (a.val_type == Val_t::NUM)
                a.val = eval_unary(curr->op, std::stod(a.val));
            else if (a.val_type == Val_t::GROUP)
                a.val = eval_unary(curr->op, std::stod(a.val));
            stack.push(a);
            break;
        case KEYWORD:
            // do nothing here
            break;
    }
    return;
}

std::string
Expr::eval_binary(Token const &tok, double a, double b)
{
    double res = 0;
    switch (tok.type) {
        case MINUS: res = a - b; break;
        case PLUS: res = a + b; break;
        case SLASH: res = a / b; break;
        case STAR: res = a * b; break;
        default: SMOL::error(tok, "eval_binary: Unimplemented operator.");
    }
    return std::to_string(res);
}

std::string
Expr::eval_binary(Token const &tok, std::string a, std::string b)
{
    std::string res("");
    switch (tok.type) {
        case PLUS: res = a + b; break;
        default: Parser::error(tok, "eval_binary: Unexpected operator on strings.");
    }
    return res;
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

// transfers the type and value from the defined symbol into the current
// expression
// Assumes curr.val is a key to a symbol in Intpr::sym_table
// Assumes the symbol exists in the table and is valid
void
Expr::swap_with_sym(Eval &curr, SymTable const &sym_table)
{
    auto var = (Var *)sym_table.get(curr.val);
    switch (var->vtype) {
        case Var_t::NUM:
            {
                double num = var->val_num;
                curr.val = std::to_string(num);
                curr.val_type = Val_t::NUM;
            }
            break;
        case Var_t::STR:
            {
                std::string str = var->val_str;
                curr.val = str;
                curr.val_type = Val_t::STR;
            }
            break;
        case Var_t::BOOL:
            {
                bool str = var->val_bool;
                curr.val = str ? "true" : "false";
                curr.val_type = Val_t::BOOL;
            }
            break;
    }
}

void
Expr::sym_undefined_exit(Expr const *curr)
{
    Intpr::error(curr, "Symbol not defined at eval-time.");
    exit(-1); // TODO: Find out an appropriate error code
}

std::string
Expr::to_string(bool b)
{
    return b ? "true" : "false";
}

std::string
Expr::to_string(Val_t type)
{
    switch(type) {
        case Val_t::NUM: return "NUM";
        case Val_t::STR: return "STR";
        case Val_t::BOOL: return "BOOL";
        case Val_t::IDENT: return "IDENT";
        case Val_t::OP: return "OP";
        case Val_t::NIL: return "NIL";
        case Val_t::GROUP: return "GROUP";
    }
}

void
Expr::print_tree(Expr *curr, std::string &res)
{
    if (curr->left) print_tree(curr->left, res);
    if (curr->right) print_tree(curr->right, res);
    res += curr->val + ", ";
}

Expr::Expr(Expr *left, Token op, Expr *right)
    : type(BINARY), val_type(Val_t::OP), val(op.lexeme)
    , left(left), op(op), right(right) {};
Expr::Expr(Token op, Expr *right)
    : type(UNARY), val_type(Val_t::OP), val(op.lexeme)
    , left(nullptr), op(op), right(right) {};
Expr::Expr(Expr *expr)
    : type(GROUPING), val_type(Val_t::GROUP), val("GROUPING")
    , left(expr), right(nullptr) {};
Expr::Expr(double num)
    : type(LITERAL), val_type(Val_t::NUM), val(std::to_string(num))
    , left(nullptr), right(nullptr) {};
Expr::Expr(std::string sym)
    : type(LITERAL), val_type(Val_t::STR), val(sym)
    , left(nullptr), right(nullptr) {};
Expr::Expr(bool b)
    : type(LITERAL), val_type(Val_t::BOOL), val(to_string(b))
    , left(nullptr), right(nullptr) {};

Expr::Expr(std::string keyword, TokenType type)
    : type(KEYWORD), val_type(Val_t::STR), val(keyword)
      , left(nullptr), right(nullptr) { (void) type; };

// used for identifiers
Expr::Expr(std::string sym, ExprType type)
    : type(LITERAL), val_type(Val_t::IDENT), val(sym)
    , left(nullptr), right(nullptr) { (void) type; };

