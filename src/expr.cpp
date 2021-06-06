#include "expr.h"
#include <iostream>

void
Expr::interpret(Expr *curr, Eval &res)
{
    switch (curr->type) {
        case BINARY:
            switch (curr->op.type) {
                case MINUS:
                    res.num = std::get<double>(curr->left->val) -
                        std::get<double>(curr->right->val);
                    break;
                case PLUS:
                    res.num = std::get<double>(curr->left->val) +
                        std::get<double>(curr->right->val);
                    break;
                case SLASH:
                    res.num = std::get<double>(curr->left->val) /
                        std::get<double>(curr->right->val);
                    break;
                case STAR:
                    res.num = std::get<double>(curr->left->val) *
                        std::get<double>(curr->right->val);
                    break;
            }
            break;
        case UNARY: break;
        case GROUPING: break;
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
