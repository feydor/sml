#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "env.h"
#include "fntable.h"
#include "ansi.h"
#include "lib.h"
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

bool SMOL::had_error = false;
bool SMOL::is_repl = false;
bool SMOL::benchmark = false;
std::string SMOL::fname;

int main(int argc, char **argv) 
{
    char c;
    int i;
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
    i = (argc == 3) ? 2 : 1;

    if (argc > 1)
        SMOL::run_file(argv[i]);
    else
        SMOL::run_prompt();
    return 0;
}

void SMOL::run_file(std::string const &fname)
{
    SMOL::fname = fname;
    SMOL::is_repl = false;
    std::stringstream buf;

    std::fstream file;
    file.exceptions(std::fstream::failbit | std::fstream::badbit);
    try {
        file.open(fname);
        buf << file.rdbuf();
        SMOL::eval(buf.str());
    } catch (std::fstream::failure& e) {
        std::cerr << "Exception opening/reading file.\n";
    }

    /* reached after evaluation */
    if (SMOL::had_error)
        exit(65);
}

void SMOL::run_prompt()
{
    std::cout << PROJECT_NAME << " " << VERSION << std::endl;
    std::string line("");
    SMOL::is_repl = true;
    for (;;) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line.empty() || line.compare("quit") == 0)
            break;
        SMOL::eval(line + "\n");
        SMOL::had_error = false;
    }
}

void SMOL::error(int line, std::string const &msg)
{
    ANSI::Modifier err(Color::FG_RED);
    ANSI::Modifier secondary(Color::FG_BLUE);
    ANSI::Modifier bold(Format::BOLD);
    ANSI::Modifier def(Color::FG_DEFAULT);
    std::string fileloc("");

    if (!SMOL::is_repl)
        fileloc = SMOL::fname + ":" + std::to_string(line);

    std::cout << err << "error" << def << ": " << bold <<
        msg << "\n " << secondary << "--> " << def << fileloc
        << std::endl;

    SMOL::had_error = true;
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
    std::vector<Tok> tokens = lexer.scan_tokens();

    Parser::parser parser(tokens);
    std::vector<Ast::Stmt *> stmts;
    try {
        stmts = parser.scan_program();
    } catch (Smol::SyntaxError& e) {
        e.print();
        exit(1);
    }

    // set prelude constants and library functions
    // TODO: Move to prelude.h
    Env::set_var(std::string("PI"), std::make_unique<Obj::Number>(3.14159265359));
    FnTable::set_fn(new Lib::to_str());
    FnTable::set_fn(new Lib::fopen());
    FnTable::set_fn(new Lib::smol_exit());

    std::chrono::system_clock::time_point t1, t2;
    if (SMOL::benchmark) {
        t1 = std::chrono::high_resolution_clock::now();
    }

    for (auto& stmt : stmts) {
        try {
            stmt->exec();
            delete stmt;
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

    FnTable::free_fns();
}
