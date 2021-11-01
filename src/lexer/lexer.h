#ifndef LEXER_H
#define LEXER_H
#include "token.h"
#include "smol_error.h"
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>

namespace Lexer {
class lexer {
    std::string src;
    std::vector<Token> tokens;
    std::unordered_map<std::string, TokenType::type> keywords;
    std::vector<Smol::LexerError> errors;
    int start = 0; // first char of current lexeme in src file
    int curr = 0; // curr char of src file before consuming
    int line = 1; // curr line of src file

    public:
    explicit lexer(std::string const& src);
    std::vector<Token> scan_tokens();
    std::vector<Smol::LexerError> get_errors();

    private:
    char advance();
    void add_string_literal(const std::string &literal);
    void add_number_literal(double num);
    void add_identifier(const std::string &identifier);
    void add_keyword(const std::string &keyword, TokenType::type type);
    bool add_identifier_as_keyword(const std::string &lexeme);
    char peek();
    char double_peek();
    bool next_is(char c);
    void str();
    void num();
    void identifier();
    bool at_end();
    static bool is_digit(char c);
    static bool is_alpha(char c);
    static bool is_alphanumeric(char c);

    void add_error(const std::string& msg);
};
}

#endif

