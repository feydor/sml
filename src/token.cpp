#include "token.h"

Token::Token(TokenType type, std::string lexeme, int line)
    : type(type), lexeme(std::move(lexeme)), line(line)
{
    this->literal = 0.0;
}

Token::Token(TokenType type, std::string lexeme, int line,
    std::variant<double, std::string> literal)
    : type(type), lexeme(std::move(lexeme)), line(line), 
    literal(std::move(literal))
{}

std::string Token::to_string() 
{
    return this->lexeme;
}
