#ifndef SMOL_H
#define SMOL_H
#include <memory>
#include <string>
#include "llvm-includes.h"
#include "jit.hpp"

struct SMOL {
    public:
        SMOL(std::unique_ptr<llvm::LLVMContext> TheContext,
             std::unique_ptr<llvm::IRBuilder<>> Builder,
             std::unique_ptr<llvm::Module> TheModule,
             std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM,
             std::unique_ptr<SmolJIT> TheJIT,
             std::map<std::string, llvm::Value *> &NamedValues)
        : TheContext(std::move(TheContext)), Builder(std::move(Builder)),
          TheModule(std::move(TheModule)), TheFPM(std::move(TheFPM)), TheJIT(std::move(TheJIT)),
          NamedValues(NamedValues) {}

        SMOL() {
            // init llvm context
            TheContext = std::make_unique<llvm::LLVMContext>();
            TheModule = std::make_unique<llvm::Module>("smol", TheContext.get());
            Builder = std::make_unique<llvm::IRBuilder<>>(TheContext.get());
            TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(TheModule);
            configure_FPM(TheFPM.get());
            NamedValues = std::map<std::string, llvm::Value *>();
        }

    private:
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
        std::string fname;

        void run_prompt();
        void run_file(std::string const &fname);
        void eval(std::string const &src);
        static void print_usage();
        static void print_version();
        void configure_FPM(llvm::legacy::FunctionPassManager *TheFPM);
        void init_jit(SmolJIT *JIT);
};

#endif

