#include "expr.h"
#include <stdexcept>

Value*
NumberExprAST::codegen()
{
    return ConstantFP::get(TheContext, APFloat(val));
}

Value*
VariableExprAST::codegen()
{
    Value *val = NamedValues[name]; // look up this var
    if (!val)
        throw std::invalid_argument("Unknown variabel name.");
    return val;
}

Value*
BinaryExprAST::codegen()
{
    Value *L = LHS->codegen();
    Value *R = RHS->codegen();
    if (!L || !R) return nullptr;

    switch(op[0]) {
        case '+': return Builder.CreateFAdd(L, R, "addtmp");
        case '-': return Builder.CreateFSub(L, R, "subtmp");
        case '*': return Builder.CreateFMul(L, R, "multmp");
        case '<': {
            L = Builder.CreateFCmpULT(L, R, "cmptmp");

            // Convert bool 0/1 to double 0.0 or 1.0
            return Builder.CreateUIToFP(L, Type::getDoubleTy(TheContext), "booltmp");
        }
        default: throw std::invalid_argument("Unknown binary operator.");
    }
}

Value*
CallExprAST::codegen()
{
    // look up name in global module table
    Function *calleef = TheModule->getFunction(callee);
    if (!calleef)
        throw std::invalid_argument("Unknown function called.");

    if (calleef->arg_size() != args.size())
        throw std::invalid_argument("Incorrect # of arguments passed.");

    std::vector<Value *> args_v;
    for (unsigned i = 0, e = args.size(); i != e; ++i) {
        args_v.push_back(args[i]->codegen());
        if (!args_v.back())
            return nullptr;
    }
    return Builder.CreateCall(calleef, args_v, "calltmp");
}



