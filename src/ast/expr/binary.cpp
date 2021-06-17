#include "expr.h"
#include <iostream>
#include <functional>
using namespace Ast;

Token
Binary::op() const
{
    return this->_op;
}

std::string
Binary::to_str() const
{
    return left()->to_str() + " " + op().to_str() + " " + right()->to_str();
}

// logical operators are only effective on num and bool types
// return NIL on any other types
template<typename Op>
Val::Val
Binary::eval_logical(Val::Val const &a, Val::Val const &b, Op fn) {
    if (a.is_num()) {
        if (b.is_num())
            return Val::Val(fn(a.get_num(), b.get_num()));
        else if (b.is_bool())
            return Val::Val(fn(a.get_num(), b.get_bool()));
    } else if (a.is_bool()) {
        if (b.is_num())
            return Val::Val(fn(a.get_bool(), b.get_num()));
        else if (b.is_bool())
            return Val::Val(fn(a.get_bool(), b.get_bool()));
    }
    std::cout << "Inptr::error: Attempted logical operation on non-num or non-bool.\n";
  return Val::Val(); // NIL
}

// assumes a and b have the same val type
Val::Val
Binary::eval(Val::Val const &a, Token const &op, Val::Val const &b)
{
    switch (op.type()) {
        case MINUS: return a - b;
        case PLUS: return a + b;
        case SLASH: return a / b;
        case STAR: return a * b;
        case AND: return eval_logical(a, b, std::logical_and());
        case OR: return eval_logical(a, b, std::logical_or());
        case BANG_EQUAL: return eval_logical(a, b, std::not_equal_to());
        case EQUAL_EQUAL: return eval_logical(a, b, std::equal_to());
        case LESS: return eval_logical(a, b, std::less());
        case LESS_EQUAL: return eval_logical(a, b, std::less_equal());
        case GREATER: return eval_logical(a, b, std::greater());
        case GREATER_EQUAL: return eval_logical(a, b, std::greater_equal());
        default: 
            // SMOL::error(op, "BINARY::eval: Unimplemented operator.");
            std::cout << "BINARY:eval: Unimplemented operator.\n";
            return Val::Val();
            break;
    }  
    return Val::Val(); // NIL value
}
