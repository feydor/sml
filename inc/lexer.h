#ifndef LEXER_H
#define LEXER_H
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include "token.h"

class Lexer {
    std::string src;
    std::vector<Token> tokens;
    std::unordered_map<std::string, TokenType> keywords;
    int start = 0; // first char of current lexeme in src file
    int curr = 0; // curr char of src file before consuming
    int line = 1; // curr line of src file

    public:
    Lexer(std::string const& src);
    std::vector<Token> scan_tokens();

    private:
    char advance();
    void add_token(TokenType type);
    void add_token(TokenType type, std::variant<double, std::string> const& lit);
    char peek();
    char peek_next();
    bool next_is(char c);
    void str();
    void num();
    void identifier();
    bool at_end();
    static bool is_digit(char c);
    static bool is_alpha(char c);
    static bool is_alphanumeric(char c);
};

#endif

