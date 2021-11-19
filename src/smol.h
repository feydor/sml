#ifndef SMOL_H
#define SMOL_H
#include <memory>
#include <string>
#include "llvm-includes.h"

struct SMOL {
    static bool had_error;
    static bool is_repl;
    static bool benchmark;
    static std::string fname;

    SMOL() {};
    static void run_prompt();
    static void run_file(std::string const &fname);
    static void eval(std::string const &src, llvm::LLVMContext &TheContext,
                                             llvm::IRBuilder<> &Builder,
                                             llvm::Module *TheModule,
                                             llvm::legacy::FunctionPassManager *TheFPM,
                                             std::map<std::string, llvm::Value *> &NamedValues);
    static void print_usage();
    static void print_version();
    static void configure_FPM(llvm::legacy::FunctionPassManager *TheFPM);
};

#endif

