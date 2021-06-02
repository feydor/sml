#ifndef LEXER_H
#define LEXER_H
#include <string>

struct Lexer {
    std::string src;
    Lexer(std::string const& src);
};

#endif

