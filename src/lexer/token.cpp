#include "token.h"
#include <stdexcept>

Token
Token::make_string_literal(const std::string &literal, int line)
{
    return Token(TokenType::STRING, literal, line);
}

Token
Token::make_num_literal(double num, int line)
{
    return Token(TokenType::NUMBER, std::to_string(num), line, num);
}

Token
Token::make_identifier(const std::string& identifier, int line)
{
    return Token(TokenType::IDENTIFIER, identifier, line);
}

Token
Token::make_keyword(TokenType::type keyword_type, const std::string& keyword, int line)
{
    return Token(keyword_type, keyword, line);
}

// use for string literals and identifiers
Token::Token(TokenType::type type, const std::string &lexeme, int line)
    : type_(type), lexeme_(lexeme), line_(line), value_(lexeme) {}

// used for numeral literals
Token::Token(TokenType::type type, const std::string &lexeme, int line, double nval)
    : type_(type), lexeme_(lexeme), line_(line), value_(nval) {}

TokenType::type
Token::get_type() const
{
    return type_;
}

std::string
Token::get_lexeme() const
{
    return lexeme_;
}

int
Token::get_line() const
{
    return line_;
}

double
Token::get_num() const
{
    return std::get<double>(value_);
}

std::string
Token::get_identifier() const
{
    return std::get<std::string>(value_);
}

std::string
Token::to_str() const
{
    return lexeme_;
}

std::string
Token::type_to_string() const
{
    switch (this->get_type()) {
        case TokenType::LEFT_PAREN: return "(";
        case TokenType::RIGHT_PAREN: return ")";
        case TokenType::COMMA: return ",";
        case TokenType::MINUS: return "-";
        case TokenType::PLUS: return "+";
        case TokenType::STAR: return "*";
        case TokenType::LESS_THAN: return "<";
        case TokenType::IDENTIFIER: return this->get_identifier();
        case TokenType::STRING: return this->get_lexeme();
        case TokenType::NUMBER: return std::to_string(this->get_num());
        case TokenType::DEF: return "DEF";
        case TokenType::EXTERN: return "EXTERN";
        case TokenType::_EOF: return "EOF";
        default: return "Unidentified.";
    }
}
