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


/*
TokenType::type
Token::char_to_type(char c) const
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
Token::type_to_string(TokenType::type type)
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
*/
