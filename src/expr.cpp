#include "expr.h"
#include <iostream>

/**
 * traversal from end-branches to root
 */
void
Expr::eval(Expr *curr, std::stack<double> &stack)
{
    if (curr->left) eval(curr->left, stack);
    if (curr->right) eval(curr->right, stack);
    std::cout << curr->op.lexeme << " : " << std::get<double>(curr->val) << std::endl;
    double n1 = 0, n2 = 0, res = 0;
    switch (curr->type) {
        case EXPR_NUMBER:
            stack.push(std::get<double>(curr->val));
            break;
        case BINARY:
            // pop two off stack, evaluate, and push res
            n2 = stack.top();
            stack.pop();
            n1 = stack.top();
            stack.pop();
            switch (curr->op.type) {
                case MINUS: res = n1 - n2; break;
                case PLUS: res = n1 + n2; break;
                case SLASH: res = n1 / n2; break;
                case STAR: res = n1 * n2; break;
            }
            stack.push(res);
            break;
    }
}
