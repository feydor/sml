#include "lexer.h"
#include "parser.h"
#include "smol.h"
#include "ArgsParser.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include <unistd.h>

#define PROJECT_NAME "smol"
#define VERSION "0.1.0"
std::string SMOL::fname = PROJECT_NAME;

int main(int argc, char **argv) 
{
    ArgsParser args_parser(argc, argv);
    if (args_parser.cmd_opt_exists("-v") || args_parser.cmd_opt_exists("--version"))
        SMOL::print_version(), exit(0);
    else if (args_parser.cmd_opt_exists("-h") || args_parser.cmd_opt_exists("--help"))
        SMOL::print_usage(), exit(0);
    
    SMOL smol;

    if (args_parser.cmd_opt_exists("-i") || args_parser.cmd_opt_exists("--emit"))
        smol.emit_ir = true;
    
    if (args_parser.cmd_opt_exists("-b") || args_parser.cmd_opt_exists("--benchmark"))
        smol.benchmark = true;
    
    const auto &filename = args_parser.get_cmd_opt("-f");
    if (!filename.empty())
        smol.run_file(filename);
    else
        smol.run_prompt();

    return 0;
}

void SMOL::run_file(std::string const &fname)
{
    SMOL::fname = fname;
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

/* start interpretation */
void SMOL::eval(std::string const &src)
{
    Lexer::lexer lexer(src);
    std::vector<Token> tokens = lexer.scan_tokens();

    for (auto err : lexer.get_errors())
        err.print();
    
    Parser parser(tokens, *this);
    
    try {
        parser.parse_syntax(); // parser holds ownership of all statements
        code_gen(parser.get_ast());
    } catch (SmolError& e) {
        e.print();
    }
}

void SMOL::code_gen(const std::vector<std::unique_ptr<DeclarationAST>> &ast)
{
    for (auto &expr : ast) {
        expr->code_gen(*this);
        
        // print generated code
        if (SMOL::emit_ir) {
            std::cout << "Now printing IR... \n";
            TheJIT->print_machine_info();
            TheModule->print(llvm::errs(), nullptr);
        }
    }

    for (auto &func : ast) {
        func->compile(*this);
    }
}

llvm::Function* SMOL::get_function(std::string_view name)
{
    // first, check for the function in the current module
    if (auto* func = TheModule->getFunction(name))
        return func;

    // std::cout << "SMOL::get_function(" << name << ")\n";

    // otherwise, check whether we can codegen it
    auto fi = FunctionPrototypes.find(static_cast<std::string>(name));
    if (fi != FunctionPrototypes.end())
        return fi->second->code_gen(*this);
    
    return nullptr;
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

void SMOL::initialize_module_and_passmanager()
{
    TheContext = std::make_unique<llvm::LLVMContext>();
    TheModule = std::make_unique<llvm::Module>("smol", *TheContext);
    TheModule->setDataLayout(TheJIT->getDataLayout());
    
    Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
    TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());
    configure_FPM(TheFPM.get());
}

void SMOL::print_usage()
{
    std::cout << "Usage: " << PROJECT_NAME << " [OPTION] [FILE]" << std::endl;
    std::cout << "smol machine ordered language interpreter" << std::endl;
    std::cout << "Example: " << PROJECT_NAME << " -f examples/pascal.smol" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -b, --benchmark   " << "activate benchmarking" << std::endl;
    std::cout << "  -i, --ir   " << "       emit IR" << std::endl;
    std::cout << "  -f   " << "             evaluate the given file" << std::endl;
    std::cout << "  -h, --help        " << "display this help text and exit" << std::endl;
    std::cout << "  -v, --version     " << "display version information and exit" << std::endl;
}

void SMOL::print_version()
{
    std::cout << PROJECT_NAME << " " << VERSION << std::endl;
    std::cout << "Copyright (C) 2021 Victor Reyes" << std::endl;
    std::cout << "This program comes with ABSOLUTELY NO WARRANTY;" << std::endl;
    std::cout << "This is free software, and you are welcome to redistribute it under certain conditions." << std::endl;
}
