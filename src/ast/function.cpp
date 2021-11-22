#include "prototype.h"
#include "smol.h"

const std::string &
PrototypeAST::get_name() const
{
    return name;
}

const std::vector<std::string> &
PrototypeAST::get_args() const
{
    return args;
}

const std::string &
FunctionAST::get_name() const
{
    return this->prototype->get_name();
}

const std::vector<std::string> &
FunctionAST::get_args() const
{
    return this->prototype->get_args();
}

llvm::Function *
PrototypeAST::code_gen(llvm::LLVMContext &Context,
                       llvm::IRBuilder<> &Builder,
                       llvm::Module* Module,
                       llvm::legacy::FunctionPassManager *FPM,
                       std::map<std::string, llvm::Value *> &namedValues)
{
    // Make the function type:  double(double,double, ...)
    std::vector<llvm::Type*> doubles(args.size(), llvm::Type::getDoubleTy(Context));
    
    llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getDoubleTy(Context), doubles, false);

    llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, name, Module);

    // set names for all args
    unsigned idx = 0;
    for (auto &arg : f->args())
        arg.setName(args[idx++]);
    return f;
}

void
PrototypeAST::compile(SMOL& compiler)
{
    // do nothing for now
}

llvm::Function *
FunctionAST::code_gen(llvm::LLVMContext &Context,
                      llvm::IRBuilder<> &Builder,
                      llvm::Module* Module,
                      llvm::legacy::FunctionPassManager *FPM,
                      std::map<std::string, llvm::Value *> &namedValues)
{
    // first, check for an existing function from a previous "extern" statement
    llvm::Function *the_function = Module->getFunction(prototype->get_name());

    if (!the_function)
        the_function = prototype->code_gen(Context, Builder, Module, FPM, namedValues);
    
    if (!the_function)
        return nullptr;
    
    if (!the_function->empty())
        throw std::invalid_argument("function cannot be redefined");

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
        llvm::verifyFunction(*the_function);
        FPM->run(*the_function); // optimize the function
        return the_function;
    }

    // error reading from body, remove function
    the_function->eraseFromParent();
    return nullptr;
}

void
FunctionAST::compile(SMOL& compiler)
{
     if (body) {
         llvm::ExitOnError llvm_exit_err;

        // Create a ResourceTracker to track JIT'd memory allocated to our
        // anonymous expression -- that way we can free it after executing
        auto RT = compiler.TheJIT->getMainJITDylib().createResourceTracker();

        auto TSM = llvm::orc::ThreadSafeModule(std::move(compiler.TheModule), std::move(compiler.TheContext));
        llvm_exit_err(compiler.TheJIT->addModule(std::move(TSM), RT));
        compiler.initialize_module_and_passmanager();

        // search the JIT for the __anon_expr symbol
        auto expr_symbol = llvm_exit_err(compiler.TheJIT->lookup("__anon_expr"));
        // assert(expr_symbol.get() && "Function not found");

        // Get the symbol's address and cast it to the right type (no args, returns double)
        // so it can be called as a native function
        double (*fp)() = (double (*)())(intptr_t)expr_symbol.getAddress();
        fprintf(stderr, "Evaluated to %f\n", fp());

        // Delete the anonymous expression module from JIT
        llvm_exit_err(RT->remove());
    }
}