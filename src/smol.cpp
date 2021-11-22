#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "smol_error.h"
#include "smol.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <unistd.h>

#define PROJECT_NAME "smol"
#define VERSION "0.1.0"

bool SMOL::benchmark = false;

int main(int argc, char **argv) 
{
    char c;
    while ((c = getopt(argc, argv, "vhb:")) != -1) {
        switch (c) {
            case 'v': SMOL::print_version(), exit(0);
            case 'h': SMOL::print_usage(), exit(0);
            case 'b': SMOL::benchmark = true; break;
            case '?':
                if (optopt == 'b')
                    std::cerr << "";
                else
                    SMOL::print_usage();
                exit(1);
            default: abort();
        }
    }
    int i = (argc == 3) ? 2 : 1;

    auto smol = std::make_unique<SMOL>();

    if (argc > 1)
        smol->run_file(argv[i]);
    else
        smol->run_prompt();
    return 0;
}

void SMOL::run_file(std::string const &fname)
{
    this->fname = fname;
    is_repl = false;
    std::stringstream buf;

    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);

    try {
        file.open(fname);
        buf << file.rdbuf();
        eval(buf.str());
    } catch (std::fstream::failure& e) {
        std::cerr << "Exception opening/reading file.\n";
    }

    /* reached after evaluation */
    if (had_error)
        exit(65);
}

void SMOL::run_prompt()
{
    std::cout << PROJECT_NAME << " " << VERSION << std::endl;
    std::string line("");
    is_repl = true;

    for (;;) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line.empty() || line.compare("quit") == 0)
            break;
        eval(line + "\n");
        had_error = false;
    }
}

void SMOL::print_usage()
{
    std::cout << "Usage: " << PROJECT_NAME << " [OPTION] [FILE]" << std::endl;
    std::cout << "smol machine ordered language interpreter" << std::endl;
    std::cout << "Example: " << PROJECT_NAME << " -b examples/pascal.smol" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -v, --version     " << "display version information and exit" << std::endl;
    std::cout << "  -b, --benchmark   " << "activate benchmarking" << std::endl;
    std::cout << "  -h, --help        " << "display this help text and exit" << std::endl;
}

void SMOL::print_version()
{
    std::cout << PROJECT_NAME << " " << VERSION << std::endl;
    std::cout << "Copyright (C) 2021 Victor Reyes" << std::endl;
    std::cout << "License MIT" << std::endl;
    std::cout << "This is free software: you are free to change and redistribute it." << std::endl;
    std::cout << "There is NO WARRANTY, to the extent permitted by law." << std::endl;
}

/* start interpretation */
void SMOL::eval(std::string const &src)
{
    Lexer::lexer lexer(src);
    std::vector<Token> tokens = lexer.scan_tokens();
    for (auto token : tokens) {
        std::cout << "| " << token.type_to_string() << " |" << " ";
    }
    std::cout << "\n";

    for (auto err : lexer.get_errors())
        err.print();
    
    Parser parser(tokens);
    
    try {
        parser.parse_syntax(); // parser holds ownership of all statements
        parser.code_gen(*TheContext.get(), *Builder.get(), TheModule.get(), TheFPM.get(), NamedValues);
    } catch (Smol::ParserError& e) {
        e.print();
        exit(1);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    /*

    // set prelude constants and library functions
    // TODO: Move to prelude.h
    Env::set_var(std::string("PI"), std::make_unique<Obj::Number>(3.14159265359));
    FnTable::set_fn(std::make_unique<Lib::to_str>());
    FnTable::set_fn(std::make_unique<Lib::fopen>());
    FnTable::set_fn(std::make_unique<Lib::smol_exit>());
    FnTable::set_fn(std::make_unique<Lib::smol_getchar>());
    FnTable::set_fn(std::make_unique<Lib::ascii>());

    std::chrono::system_clock::time_point t1, t2;
    if (SMOL::benchmark) {
        t1 = std::chrono::high_resolution_clock::now();
    }

    for (const auto& stmt : parser.view_ast()) {
        try {
            stmt->exec();
        } catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        } catch (Obj::Object& ret_outside_fn) {
            // TODO: catch a Smol::runtime_error here, but do not exit
            std::cout << "Error: return statement outside a function.\n";
        }
    }

    if (SMOL::benchmark) {
        t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms_double = t2 - t1;
        std::cout << "Duration: " << ms_double.count() << "ms\n";
    }
    */
}

void SMOL::init_jit(SmolJIT* JIT) {
    // llvm JIT initialization
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
    auto result = SmolJIT::Create();
    if (auto err = result.takeError()) {
        llvm::consumeError(std::move(err)); // TODO: handle error
    }
    this->TheJIT = std::move(*result);
    this->TheModule->setDataLayout(this->TheJIT.get()->getDataLayout());
}

// Add optimization passes to the FunctionPassManager
void SMOL::configure_FPM(llvm::legacy::FunctionPassManager *TheFPM)
{
    // Do simple "peephole" optimizations and bit-twiddling options.
    TheFPM->add(llvm::createInstructionCombiningPass());
    // Reassociate expressions
    TheFPM->add(llvm::createReassociatePass());
    // Eliminate Common SubExpressions
    TheFPM->add(llvm::createGVNPass());
    // Simplify the control flow graph (delete unreachable blocks, etc)
    TheFPM->add(llvm::createCFGSimplificationPass());

    TheFPM->doInitialization();
}
