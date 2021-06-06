#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "smol.h"

#define PROJECT_NAME "smol"
#define VERSION "v0.1.0"

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

bool SMOL::had_error = false;

void SMOL::run_file(std::string const &fname) 
{
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
    for (;;) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line.empty())
            break;
        SMOL::eval(line);
        SMOL::had_error = false;
    }
}

void SMOL::error(int line, std::string const &message)
{
    std::cout << "[line " << line << "] Error " << message << std::endl;
    SMOL::had_error = true;
}

void SMOL::error(Token const& tok, std::string const &message)
{
    if (tok.type == _EOF)
        std::cout << "[line " << tok.line << "] Error " << "at end " <<
            message << std::endl;
    else
        std::cout << "[line " << tok.line << "] Error " << "at '" <<
            tok.lexeme << "' " + message << std::endl;
    SMOL::had_error = true;
}

/* start interpretation */
void SMOL::eval(std::string const &src)
{
    
    Lexer lexer(src);
    std::vector<Token> tokens = lexer.scan_tokens();
    std::cout << "Printing tokens... ";
    for (auto token : tokens) {
        std::cout << token.to_string();
    }

    Parser parser(tokens);
    std::vector<Expr *> exprs = parser.scan_exprs();

    std::cout << "\nEvaluating expressions... ";
    Eval res;
    for (auto &expr : exprs) {
        expr->interpret(expr, res);
        switch (expr->type) {
            case BINARY:
                std::cout << res.num << std::endl;
                break;
        }
    }
}
