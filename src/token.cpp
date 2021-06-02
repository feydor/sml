#include "token.h"

Token::Token(TokenType type, std::string lexeme, int line)
    : type(type), lexeme(std::move(lexeme)), line(line)
{}

std::string Token::to_string() 
{
    return this->type + " " + this->lexeme;
}
