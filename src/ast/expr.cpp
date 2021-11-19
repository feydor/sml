#include "expr.h"
#include <stdexcept>

llvm::Value*
NumberExprAST::code_gen(llvm::LLVMContext &Context,
                        llvm::IRBuilder<> &Builder,
                        llvm::Module* Module,
                        std::map<std::string, llvm::Value *> &namedValues)
{
    return llvm::ConstantFP::get(Context, llvm::APFloat(val));
}

llvm::Value*
VariableExprAST::code_gen(llvm::LLVMContext &Context,
                          llvm::IRBuilder<> &Builder,
                          llvm::Module* Module,
                          std::map<std::string, llvm::Value *> &namedValues)
{
    llvm::Value *val = namedValues[name]; // look up this var
    if (!val)
        throw std::invalid_argument("Unknown variable name.");
    return val;
}

llvm::Value*
BinaryExprAST::code_gen(llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder,
                       llvm::Module* Module, std::map<std::string, llvm::Value *> &namedValues)
{
    llvm::Value *L = LHS->code_gen(Context, Builder, Module, namedValues);
    llvm::Value *R = RHS->code_gen(Context, Builder, Module, namedValues);
    if (!L || !R) return nullptr;

    // TODO: multi-character (non-ascii) operators?
    // store and switch by tokentype, not raw string
    switch(op[0]) {
        case '+': return Builder.CreateFAdd(L, R, "addtmp");
        case '-': return Builder.CreateFSub(L, R, "subtmp");
        case '*': return Builder.CreateFMul(L, R, "multmp");
        case '<': {
            L = Builder.CreateFCmpULT(L, R, "cmptmp"); // returns i1/boolean

            // Convert bool 0/1 to double 0.0 or 1.0
            return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(Context), "booltmp");
        }
        case '>': {
            L = Builder.CreateFCmpUGT(L, R, "cmptmp");
            return Builder.CreateUIToFP(L, llvm::Type::getDoubleTy(Context), "booltmp");
        }
        default: throw std::invalid_argument("Unknown binary operator.");
    }
}

llvm::Value*
CallExprAST::code_gen(llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder,
                     llvm::Module* Module, std::map<std::string, llvm::Value *> &namedValues)
{
    // look up name in global module table
    llvm::Function *calleef = Module->getFunction(callee);
    if (!calleef)
        throw std::invalid_argument("Unknown function called.");

    if (calleef->arg_size() != args.size())
        throw std::invalid_argument("Incorrect # of arguments passed.");

    std::vector<llvm::Value *> args_v;
    for (unsigned i = 0, e = args.size(); i != e; ++i) {
        args_v.push_back(args[i]->code_gen(Context, Builder, Module, namedValues));
        if (!args_v.back())
            return nullptr;
    }
    return Builder.CreateCall(calleef, args_v, "calltmp");
}



