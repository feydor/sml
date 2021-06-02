#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <variant>

enum TokenType {
    // single-character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, COMMA, DOT,
    MINUS, PLUS, SEMICOLON, SLASH, STAR,
    // one or two character tokens
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    // literals
    IDENTIFIER, STRING, NUMBER,
    // keywords
    AND, CLASS, ELSE, FALSE, FN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER,
    THIS, TRUE, LET, WHILE, _EOF,
};

class Token {
    TokenType type;
    std::string lexeme;
    int line;
    std::variant<double, std::string> literal;

    public:
    Token(TokenType type, std::string lexeme, int line);
    Token(TokenType type, std::string lexeme, int line,
        std::variant<double, std::string> literal);
    std::string to_string();
};

#endif

