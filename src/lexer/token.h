#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <variant>

enum Token_t {
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
    Token_t type_;
    std::string lexeme_;
    int line_;
    std::variant<double, std::string> literal_;

    public:
    Token(Token_t type, std::string lexeme, int line);
    Token(Token_t type, std::string lexeme, int line,
        std::variant<double, std::string> literal);
    Token();

    Token_t type() const;
    std::string lexeme() const;
    int line() const;
    double get_literal_num() const;
    std::string get_literal_str() const;
    std::string to_str() const;
    static std::string type_to_string(Token_t type);
};

#endif

