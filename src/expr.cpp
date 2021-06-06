#include "expr.h"
#include <iostream>

double
Expr::interpret(Expr *curr, Eval &res)
{
    if (!curr) return res.num;

    switch (curr->type) {
        case EXPR_NUMBER:
            return std::get<double>(curr->val);
        case BINARY:
            return eval_binary(curr, res);
            break;
        case UNARY: break;
        case GROUPING:
            return interpret(curr->left, res);
            break;
    } 
}

double
Expr::eval_binary(Expr *curr, Eval &res)
{
    if (!curr) return 0;
    double n1 = eval_binary(curr->left, res);
    double n2 = eval_binary(curr->right, res);

    if (curr->type == EXPR_NUMBER)
        return std::get<double>(curr->val);

    switch (curr->op.type) {
        case MINUS:
            res.num = n1 - n2;
            return res.num;
        case PLUS:
            res.num = n1 + n2;
            return res.num;
        case SLASH:
            res.num = n1 / n2;
            return res.num;
        case STAR:
            res.num = n1 * n2;
            return res.num;
    }
}

void
Expr::to_string()
{
    Expr *curr = this;
    while (curr != nullptr) {
        std::cout << Token::type_to_string(curr->op.type) << std::endl;
        curr = curr->right;
    }
}
