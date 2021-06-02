#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <vector>
#include "token.h"

struct Lexer {
    std::string src;

    Lexer(std::string const& src);
    std::vector<Token> scan_tokens();
};

#endif

