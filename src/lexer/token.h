#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <variant>

enum TokenType {
    // single-character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_BRACKET,
    RIGHT_BRACKET, COMMA, DOT,
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

class Token {
    public:
    TokenType type;
    std::string lexeme;
    int line;
    std::variant<double, std::string> literal;

    Token(TokenType type, std::string lexeme, int line);
    Token(TokenType type, std::string lexeme, int line,
        std::variant<double, std::string> literal);
    Token();
    std::string to_string();
    static std::string type_to_string(TokenType type);
};

#endif

