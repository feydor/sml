#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <variant>

namespace Token {
enum type {
    // single-character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET,
    RIGHT_BRACKET, COMMA, DOT, PERCENT,
    MINUS, PLUS, SEMICOLON, SLASH, STAR,
    // one or two character tokens
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    // literals
    IDENTIFIER, STRING, NUMBER,
    // keywords
    AND, ELSE, ELSE_IF, FALSE, FN, FOR, IF, NIL, OR, SAY, RETURN,
    TRUE, LET, WHILE, _EOF, EOL,
};
}

class Token {
    Token::type type_;
    std::string lexeme_;
    int line_;
    std::variant<double, std::string> literal_;

    public:
    Token(Token::type type, std::string lexeme, int line);
    Token(Token::type type, std::string lexeme, int line,
        std::variant<double, std::string> literal);
    Token();

    Token::type type() const;
    std::string lexeme() const;
    int line() const;
    double get_literal_num() const;
    std::string get_literal_str() const;
    std::string to_str() const;
    static std::string type_to_string(Token::type type);
};

#endif

