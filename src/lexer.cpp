#include <iostream>
#include "lexer.h"

Lexer::Lexer(std::string const& src) 
    : src(src) 
{}

std::vector<Token> Lexer::scan_tokens()
{
    std::vector<Token> v;
    for (char c : this->src) {
        std::cout << c;
    }

    return v;
}
