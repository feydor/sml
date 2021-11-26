#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <variant>
#include <memory>

namespace TokenType {
    enum type {
        _EOF,

        // commands
        DEF,
        EXTERN,

        // primary
        IDENTIFIER,
        STRING,
        NUMBER,

        // control
        IF,
        THEN,
        ELSE,

        // operators
        LESS_THAN,
        GREATER_THAN,
        PLUS,
        MINUS,
        STAR,

        // SEPERATORS
        LEFT_PAREN,
        RIGHT_PAREN,
        COMMA
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

    TokenType::type get_type() const;
    std::string get_lexeme() const;
    int get_line() const;
    double get_num() const;
    std::string get_identifier() const;
    std::string to_str() const;
    std::string type_to_string() const;

    private:
    // identifiers and string literals
    Token(TokenType::type type, const std::string &lexeme, int line);
    
    // numbers
    Token(TokenType::type type, const std::string &lexeme, int line, double nval);
    
};

#endif

