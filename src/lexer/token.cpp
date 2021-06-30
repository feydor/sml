#include "token.h"
#include <stdexcept>

Tok::Tok(Token::type type, std::string lexeme, int line)
    : type_(type), lexeme_(std::move(lexeme)), line_(line), literal_(0.0)
{}

Tok::Tok(Token::type type, std::string lexeme, int line,
    std::variant<double, std::string> literal)
    : type_(type), lexeme_(std::move(lexeme)), line_(line),
    literal_(std::move(literal))
{}

Tok::Tok(Token::type type, std::string lexeme)
    : type_(type), lexeme_(std::move(lexeme)), line_(0) {};

Tok::Tok() {}

Token::type
Tok::type() const
{
    return type_;
}

int
Tok::line() const
{
    return line_;
}

double
Tok::get_num() const
{
    return std::get<double>(literal_);
}

std::string
Tok::get_str() const
{
    return std::get<std::string>(literal_);
}

std::string
Tok::to_str() const
{
    return lexeme_;
}

Tok
Tok::first_subtok() const
{
    return Tok(char_to_type(to_str()[0]), to_str());
}

Token::type
Tok::char_to_type(char c) const
{
    switch (c) {
        case '+': return Token::PLUS;
        case '-': return Token::MINUS;
        case '/': return Token::SLASH;
        case '*': return Token::STAR;
        case '%': return Token::PERCENT;
        default: throw std::runtime_error("char_to_type: type not implemented.");
    }
}

std::string
Tok::type_to_string(Token::type type)
{
    switch (type) {
        case Token::LEFT_PAREN: return "(";
        case Token::RIGHT_PAREN: return ")";
        case Token::LEFT_BRACE: return "{";
        case Token::RIGHT_BRACE: return "}";
        case Token::LEFT_BRACKET: return "[";
        case Token::RIGHT_BRACKET: return "]";
        case Token::COMMA: return ",";
        case Token::DOT: return ".";
        case Token::MINUS: return "-";
        case Token::PLUS: return "+";
        case Token::MINUS_MINUS: return "--";
        case Token::PLUS_PLUS: return "++";
        case Token::SEMICOLON: return ";";
        case Token::SLASH: return "/";
        case Token::STAR: return "*";
        case Token::BANG: return "!";
        case Token::QUESTION: return "?";
        case Token::PERCENT: return "%";
        case Token::BANG_EQUAL: return "!=";
        case Token::EQUAL: return "=";
        case Token::EQUAL_EQUAL: return "==";
        case Token::GREATER: return ">";
        case Token::GREATER_EQUAL: return ">=";
        case Token::LESS: return "<";
        case Token::LESS_EQUAL: return "<=";
        case Token::IDENTIFIER: return "IDENTIFIER";
        case Token::STRING: return "STRING";
        case Token::NUMBER: return "NUMBER";
        case Token::AND: return "AND";
        case Token::ELSE: return "ELSE";
        case Token::ELSE_IF: return "ELSE_IF";
        case Token::FALSE: return "FALSE";
        case Token::FN: return "FN";
        case Token::FOR: return "FOR";
        case Token::IF: return "IF";
        case Token::NIL: return "NIL";
        case Token::OR: return "OR";
        case Token::SAY: return "SAY";
        case Token::RETURN: return "RET";
        case Token::TRUE: return "TRUE";
        case Token::LET: return "LET";
        case Token::WHILE: return "WHILE";
        case Token::_EOF: return "EOF";
        case Token::EOL: return "EOL";
        default: return "Not yet identifed.";
    }
}
