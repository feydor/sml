#include "prototype.h"
#include "smol.h"
#include <iostream>

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
PrototypeAST::code_gen(SMOL &smol)
{
    // Make the function type:  double(double,double, ...)
    std::vector<llvm::Type*> doubles(args.size(), llvm::Type::getDoubleTy(*smol.TheContext));
    
    llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getDoubleTy(*smol.TheContext), doubles, false);

    llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, name, *smol.TheModule);

    // set names for all args
    unsigned idx = 0;
    for (auto &arg : f->args())
        arg.setName(args[idx++]);
    return f;
}

void
PrototypeAST::compile(__attribute__((unused)) SMOL& compiler)
{
    // add to the compiler's FunctionPrototypes map
    compiler.FunctionPrototypes[get_name()] =
        std::make_unique<PrototypeAST>(get_name(), get_args());
}

llvm::Function *
FunctionAST::code_gen(SMOL &smol)
{

    // add to the compiler's FunctionPrototypes map, and make a reference of it
    auto &p = *prototype;
    smol.FunctionPrototypes[prototype->get_name()] =
        std::make_unique<PrototypeAST>(prototype->get_name(), prototype->get_args());
    llvm::Function *the_function = smol.get_function(p.get_name());

    // if (!the_function)
    //     the_function = prototype->code_gen(smol);
    
    if (!the_function)
        return nullptr;
    
    // if (!the_function->empty())
    //     throw std::invalid_argument("function cannot be redefined");

    // create a new basic block to start insertion into
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*smol.TheContext, "entry", the_function);
    smol.Builder->SetInsertPoint(BB);

    // record function arguments in the NamedValues map
    smol.NamedValues.clear();
    for (auto &arg : the_function->args())
        smol.NamedValues[std::string(arg.getName())] = &arg;
    
    // code_gen on body
    if (llvm::Value *ret_val = body->code_gen(smol)) {
        smol.Builder->CreateRet(ret_val);
        llvm::verifyFunction(*the_function);
        smol.TheFPM->run(*the_function); // optimize the function
        return the_function;
    }

    // error reading from body, remove function
    the_function->eraseFromParent();
    return nullptr;
}

void
FunctionAST::compile(SMOL& compiler)
{   
    // std::cout << "compiling this\n";

    llvm::ExitOnError llvm_exit_err;

    // std::cout << "FUNCTIONAST::COMPILE name = " << get_name() << std::endl;
    
    // handle definition
    if (this->body && get_name() != "__anon_expr") {
        // std::cout << "NO PROTOTYPE\n";
        auto RT = compiler.TheJIT->getMainJITDylib().createResourceTracker();
        auto TSM = llvm::orc::ThreadSafeModule(std::move(compiler.TheModule), std::move(compiler.TheContext));
        llvm_exit_err(compiler.TheJIT->addModule(std::move(TSM), RT));
        compiler.initialize_module_and_passmanager();
    } else
    // handle toplevel expression
    if (body) {
    
        // Create a ResourceTracker to track JIT'd memory allocated to our
        // anonymous expression -- that way we can free it after executing
        auto RT = compiler.TheJIT->getMainJITDylib().createResourceTracker();

        auto TSM = llvm::orc::ThreadSafeModule(std::move(compiler.TheModule), std::move(compiler.TheContext));
        llvm_exit_err(compiler.TheJIT->addModule(std::move(TSM), RT));
        compiler.initialize_module_and_passmanager();

        // search the JIT for the __anon_expr symbol
        auto expr_symbol = llvm_exit_err(compiler.TheJIT->lookup(this->get_name()));
        // assert(expr_symbol.get() && "Function not found");

        // Get the symbol's address and cast it to the right type (no args, returns double)
        // so it can be called as a native function
        double (*fp)() = (double (*)())(intptr_t)expr_symbol.getAddress();
        fprintf(stderr, "Evaluated to %f\n", fp());

        // Delete the anonymous expression module from JIT
        llvm_exit_err(RT->remove());
    }
}