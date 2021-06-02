#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include "lexer.h"
#include "token.h"
#include "sml.h"

#define PROJECT_NAME "sml"
#define VERSION "v0.1.0"

int main(int argc, char **argv) 
{
    // std::unique_ptr<SML> sml(new SML());
    if (argc > 2) {
        std::cout << "Usage: " << PROJECT_NAME << " [source]" << "\n";
        exit(64);
    } else if (argc == 2) {
        SML::run_file(argv[1]);
    } else {
        SML::run_prompt();
    }
    return 0;
}

bool SML::had_error = false;

SML::SML() 
{}

void SML::run_file(std::string const& fname) 
{
    std::fstream ifs(fname);
    std::stringstream buf;
    buf << ifs.rdbuf();
    SML::eval(buf.str());

    /* reached after evaluation */
    if (SML::had_error)
        exit(65);
}

void SML::run_prompt()
{
    std::cout << PROJECT_NAME << " " << VERSION << std::endl;
    std::string line("");
    for (;;) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line.empty())
            break;
        SML::eval(line);
        SML::had_error = false;
    }
}

void SML::error(int line, std::string const& message)
{
    std::cout << "[line " << line << "] Error " << message << std::endl;
    this->had_error = true;
}

/* start interpretation */
void SML::eval(std::string const& src)
{
    std::unique_ptr<Lexer> lexer(new Lexer(src));
    std::vector<Token> tokens = lexer->scan_tokens();
    for (auto token : tokens) {
        std::cout << token.to_string() << std::endl;
    }
}
