#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lexer.h"
#include "token.h"

#define PROJECT_NAME "sml"
#define VERSION "v0.1.0"

void run_file(std::string fname);
void run_prompt();
void eval(std::string line);

int main(int argc, char **argv) 
{
    if (argc > 2) {
        std::cout << "Usage: " << PROJECT_NAME << " [source]" << "\n";
        exit(64);
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        run_prompt();
    }
    return 0;
}

void run_file(std::string fname) 
{
    std::fstream ifs(fname);
    std::stringstream buf;
    buf << ifs.rdbuf();
    eval(buf.str());
}

void run_prompt()
{
    std::cout << PROJECT_NAME << " " << VERSION << std::endl;
    std::string line("");
    for (;;) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line.empty())
            break;
        eval(line);
    }
}

/* start interpretation */
void eval(std::string src) {
    Lexer lexer = new Lexer(src);
    std::vector<Token> tokens = scanner.scan_tokens();
    for (auto token : tokens) {
        std::cout << token << std::endl;
    }
}
