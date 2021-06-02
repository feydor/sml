#include "token.h"

Token::Token(TokenType type, std::string const& lexeme, int line)
    : type(type)
    : lexeme(lexeme)
    : line(line)
{}

std::string Token::to_string() 
{
    return this->type + " " + this->lexeme;
}
