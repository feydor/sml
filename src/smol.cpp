#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "env.h"
#include "fntable.h"
#include "ansi.h"
#include "lib.h"
#include "retstack.h"
#include "smol_error.h"
#include "smol.h"

#define PROJECT_NAME "smol"
#define VERSION "0.1.0"

bool SMOL::had_error = false;
bool SMOL::is_repl = false;
std::string SMOL::fname("");

int main(int argc, char **argv) 
{
    if (argc > 2) {
        std::cout << "Usage: " << PROJECT_NAME << " [source]" << "\n";
        exit(64);
    } else if (argc == 2) {
        SMOL::run_file(argv[1]);
    } else {
        SMOL::run_prompt();
    }
    return 0;
}

void SMOL::run_file(std::string const &fname) 
{
    SMOL::fname = fname;
    SMOL::is_repl = false;

    std::fstream ifs(fname);
    std::stringstream buf;
    buf << ifs.rdbuf();
    SMOL::eval(buf.str());

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

    std::cout << "Evaluating statements (stmts_size: " 
        << stmts.size() << ")...";

    std::cout << "Begin interpretation...\n";

    // set prelude constants and library functions
    // TODO: Move to prelude.h
    Env::set_var(std::string("PI"), Val(3.14159265359));
    FnTable::set_fn(new Lib::to_str());

    for (auto& stmt : stmts) {
        try {
            stmt->exec();
            delete stmt;
        } catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
    }

    if (!RetStack::is_empty()) {
        std::cout << "err: ReturnStack was not empty upon exiting statement execution./n";
        std::cout << RetStack::to_str();
    }
}
