#ifndef SMOL_H
#define SMOL_H
#include <memory>
#include <string>
#include "llvm-includes.h"
#include "jit.hpp"
#include "prototype.h"

struct SMOL {
    public:
        SMOL() {
            // init llvm context
            TheContext = std::make_unique<llvm::LLVMContext>();
            TheModule = std::make_unique<llvm::Module>("smol", *TheContext);
            Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
            TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());
            configure_FPM(TheFPM.get());
            NamedValues = std::map<std::string, llvm::Value *>();
         
            // llvm JIT initialization
            llvm::InitializeNativeTarget();
            llvm::InitializeNativeTargetAsmPrinter();
            llvm::InitializeNativeTargetAsmParser();

            llvm::ExitOnError exitonerr;
            TheJIT = exitonerr(SmolJIT::Create());
            TheModule->setDataLayout(TheJIT.get()->getDataLayout());
        }

    public:
        std::unique_ptr<llvm::LLVMContext> TheContext;
        std::unique_ptr<llvm::IRBuilder<>> Builder;
        std::unique_ptr<llvm::Module> TheModule;
        std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM;
        std::unique_ptr<SmolJIT> TheJIT;
        std::map<std::string, llvm::Value *> NamedValues;

    public:
        static bool benchmark;
        bool had_error;
        bool is_repl;
        static std::string fname;

        void run_prompt();
        void run_file(std::string const &fname);
        void eval(std::string const &src);
        void code_gen(const std::vector<std::unique_ptr<DeclarationAST>> &ast);
        static void print_usage();
        static void print_version();
        void configure_FPM(llvm::legacy::FunctionPassManager *TheFPM);
        void initialize_module_and_passmanager();
};

#endif

