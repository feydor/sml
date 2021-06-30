#ifndef LEXER_H
#define LEXER_H
#include "token.h"
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>

namespace Lexer {
class lexer {
    std::string src;
    std::vector<Tok> tokens;
    std::unordered_map<std::string, Token::type> keywords;
    int start = 0; // first char of current lexeme in src file
    int curr = 0; // curr char of src file before consuming
    int line = 1; // curr line of src file

    public:
    explicit lexer(std::string const& src);
    std::vector<Tok> scan_tokens();

    private:
    char advance();
    void add_token(Token::type type);
    void add_token(Token::type type, std::variant<double, std::string> const& lit);
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
}

#endif

