#include "prototype.h"
#include <map>
#include <iostream>

std::string
FunctionAST::get_name() const
{
    return prototype->get_name();
}

std::vector<std::string>
FunctionAST::get_args() const
{
    return prototype->get_args();
}

llvm::Function*
PrototypeAST::code_gen(llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder,
                       llvm::Module* Module, std::map<std::string, llvm::Value *> &namedValues)
{
    // Make the function type:  double(double,double) etc.
    std::vector<llvm::Type*> doubles(args.size(), llvm::Type::getDoubleTy(Context));

    llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getDoubleTy(Context), doubles, false);

    llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, name, Module);

    // set names for all args
    unsigned idx = 0;
    for (auto &arg : f->args())
        arg.setName(args[idx++]);
    return f;
}

llvm::Function*
FunctionAST::code_gen(llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder,
                      llvm::Module* Module, std::map<std::string, llvm::Value *> &namedValues)
{
    // first, check for an existing function from a previous "extern" statement
    llvm::Function *the_function = Module->getFunction(prototype->get_name());

    if (!the_function)
        the_function = prototype->code_gen(Context, Builder, Module, namedValues);
    
    if (!the_function)
        return nullptr;
    
    if (!the_function->empty())
        throw std::invalid_argument("function cannot be redfined");

    // create a new basic block to start insertion into
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(Context, "entry", the_function);
    Builder.SetInsertPoint(BB);

    // record function arguments in the NamedValues map
    namedValues.clear();
    for (auto &arg : the_function->args())
        namedValues[std::string(arg.getName())] = &arg;
    
    // code_gen on body
    if (llvm::Value *ret_val = body->code_gen(Context, Builder, Module, namedValues)) {
        Builder.CreateRet(ret_val);
        verifyFunction(*the_function);
        return the_function;
    }

    // error reading from body, remove function
    the_function->eraseFromParent();
    return nullptr;
}