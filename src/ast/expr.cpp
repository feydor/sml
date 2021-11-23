#include "expr.h"
#include <stdexcept>
#include <iostream>

llvm::Value*
NumberExprAST::code_gen(SMOL &smol)
{
    return llvm::ConstantFP::get(*smol.TheContext, llvm::APFloat(val));
}

llvm::Value*
VariableExprAST::code_gen(SMOL &smol)
{
    llvm::Value *val = smol.NamedValues[name]; // look up this var
    if (!val)
        throw std::invalid_argument("Unknown variable name.");
    return val;
}

llvm::Value*
BinaryExprAST::code_gen(SMOL &smol)
{
    llvm::Value *L = LHS->code_gen(smol);
    llvm::Value *R = RHS->code_gen(smol);
    if (!L || !R) return nullptr;

    // TODO: multi-character (non-ascii) operators?
    // store and switch by tokentype, not raw string
    switch(op[0]) {
        case '+': return smol.Builder->CreateFAdd(L, R, "addtmp");
        case '-': return smol.Builder->CreateFSub(L, R, "subtmp");
        case '*': return smol.Builder->CreateFMul(L, R, "multmp");
        case '<': {
            L = smol.Builder->CreateFCmpULT(L, R, "cmptmp"); // returns i1/boolean

            // Convert bool 0/1 to double 0.0 or 1.0
            return smol.Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*smol.TheContext), "booltmp");
        }
        case '>': {
            L = smol.Builder->CreateFCmpUGT(L, R, "cmptmp");
            return smol.Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*smol.TheContext), "booltmp");
        }
        default: throw std::invalid_argument("Unknown binary operator.");
    }
}

llvm::Value*
CallExprAST::code_gen(SMOL &smol)
{
    // look up name in global module table
    llvm::Function *calleef = smol.get_function(callee);
    
    // std::cout << "CallExprAST::code_gen: " << callee << std::endl;

    if (!calleef)
        throw std::invalid_argument("Unknown function called.");

    if (calleef->arg_size() != args.size())
        throw std::invalid_argument("Incorrect # of arguments passed.");

    std::vector<llvm::Value *> args_v;
    for (unsigned i = 0, e = args.size(); i != e; ++i) {
        args_v.push_back(args[i]->code_gen(smol));
        if (!args_v.back())
            return nullptr;
    }
    return smol.Builder->CreateCall(calleef, args_v, "calltmp");
}



