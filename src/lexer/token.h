#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <variant>
#include <memory>

namespace TokenType {
    enum type {
        _EOF = -1,

        // commands
        DEF = -2,
        EXTERN = -3,

        // primary
        IDENTIFIER = -4,
        STRING = -5,
        NUMBER = -6,
    };
}

class Token {
    TokenType::type type_;
    std::string lexeme_;
    int line_;
    std::variant<double, std::string> value_;

    public:
    static Token make_string_literal(const std::string &literal, int line);
    static Token make_num_literal(double num, int line);
    static Token make_identifier(const std::string& identifier, int line);
    static Token make_keyword(TokenType::type keyword_type, const std::string& identifier, int line);

    TokenType::type type() const;
    std::string lexeme() const;
    int line() const;
    double get_num() const;
    std::string get_identifier() const;
    std::string to_str() const;
    static std::string type_to_string(TokenType::type type);

    private:
    // identifiers and string literals
    Token(TokenType::type type, const std::string &lexeme, int line);
    
    // numbers
    Token(TokenType::type type, const std::string &lexeme, int line, double nval);
    
};

#endif

