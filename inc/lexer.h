#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <vector>
#include "token.h"

class Lexer {
    std::string src;
    std::vector<Token> tokens;
    int start = 0; // first char of current lexeme in src file
    int curr = 0; // curr char of src file before consuming
    int line = 1; // curr line of src file

    public:
    Lexer(std::string const& src);
    std::vector<Token> scan_tokens();

    private:
    char advance();
    void add_token(TokenType type);
    char peek();
    bool next_is(char c);
    bool at_end();
};

#endif

