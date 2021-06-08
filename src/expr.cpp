#include "expr.h"
#include "parser.h"
#include "smol.h"
#include <iostream>
#include <cstring>
#include <string>


/**
 * traversal from end-branches to root
 */
void
Expr::eval(Expr *curr, std::stack<Literal> &stack)
{
    if (curr->left) eval(curr->left, stack);
    if (curr->right) eval(curr->right, stack);
    std::cout << curr->op.lexeme << " : " << curr->val << std::endl;
    Literal a, b, res;
    switch (curr->type) {
        case LITERAL:
            res.val_type = curr->val_type;
            res.val = curr->val;
            stack.push(res);
            break;
        case BINARY:
            // pop two off stack, evaluate, and push res
            b = stack.top();
            stack.pop();
            a = stack.top();
            stack.pop();

            res.val_type = a.val_type;
            if (a.val_type != b.val_type) {
                Parser::error(curr->op, 
                        "Expr::eval: Literal types do not match.");
                res.val = "ERR";
                res.val_type = VAL_NIL;
                stack.push(res);
                return;
            }

            // eval number or string
            if (a.val_type == VAL_NUMBER)
                res.val = eval_binary(curr->op, 
                        std::stod(a.val), std::stod(b.val));
            else
                res.val = eval_binary(curr->op, a.val, b.val);

            stack.push(res);
            break;
    }
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
        default: SMOL::error(tok, "Expr::eval: Unimplemented operator.");
    }
    return std::to_string(res);
}

std::string
Expr::eval_binary(Token const &tok, std::string a, std::string b)
{
    std::string res("");
    switch (tok.type) {
        case PLUS: res = a + b; break;
        default: Parser::error(tok, "Expr::eval: Unexpected operator on strings.");
    }
    return res;
}

std::string to_string(bool b) {
    return b ? "true" : "false";
}

Expr::Expr(Expr *left, Token op, Expr *right)
    : type(BINARY), val_type(VAL_OP), val(op.lexeme)
    , left(left), op(op), right(right) {};
Expr::Expr(Token op, Expr *right)
    : type(UNARY), val_type(VAL_OP), val(op.lexeme)
    , left(nullptr), op(op), right(right) {};
Expr::Expr(Expr *expr)
    : type(GROUPING), val_type(VAL_GROUP), val("GROUPING")
    , left(expr), right(nullptr) {};
Expr::Expr(double dbl)
    : type(LITERAL), val_type(VAL_NUMBER), val(std::to_string(dbl))
    , left(nullptr), right(nullptr) {};
Expr::Expr(std::string str)
    : type(LITERAL), val_type(VAL_STRING), val(str)
    , left(nullptr), right(nullptr) {};
Expr::Expr(bool b)
    : type(LITERAL), val_type(VAL_BOOL), val(to_string(b))
    , left(nullptr), right(nullptr) {};

