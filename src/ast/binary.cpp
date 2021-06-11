#include "expr.h"

namespace Ast {

Val::Val
::eval()
{
    return this->left->eval();

    /*
    double res = 0;
    switch (tok.type) {
        case MINUS: res = a - b; break;
        case PLUS: res = a + b; break;
        case SLASH: res = a / b; break;
        case STAR: res = a * b; break;
        default: SMOL::error(tok, "eval_binary: Unimplemented operator.");
    }
    return std::to_string(res);
    */
}

}
