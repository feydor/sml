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
    LESS, LESS_EQUAL, PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, SLASH_EQUAL,
    PERCENT_EQUAL, PLUS_PLUS, MINUS_MINUS,
    // literals
    IDENTIFIER, STRING, NUMBER,
    // keywords
    AND, ELSE, ELSE_IF, FALSE, FN, FOR, IF, NIL, OR, SAY, RETURN,
    TRUE, LET, WHILE, _EOF, EOL,
};
}

class Tok {
    Token::type type_;
    std::string lexeme_;
    int line_;
    std::variant<double, std::string> literal_;

    public:
    Tok(Token::type type, std::string lexeme, int line);
    Tok(Token::type type, std::string lexeme, int line,
        std::variant<double, std::string> literal);
    Tok();

    // for building a token outside of lexer
    Tok(Token::type type, std::string lexeme);

    Token::type type() const;
    std::string lexeme() const;
    int line() const;
    double get_num() const;
    std::string get_str() const;
    std::string to_str() const;
    Tok first_subtok() const;
    static std::string type_to_string(Token::type type);

    private:
    Token::type char_to_type(char c) const;
};

#endif

