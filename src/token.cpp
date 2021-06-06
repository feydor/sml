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

Token::Token() {}

std::string Token::to_string() 
{
    return this->lexeme;
}

std::string Token::type_to_string(TokenType type) {
    switch (type) {
        case LEFT_PAREN: return "(";
        case RIGHT_PAREN: return ")";
        case LEFT_BRACE: return "{";
        case RIGHT_BRACE: return "}";
        case COMMA: return ",";
        case DOT: return ".";
        case MINUS: return "-";
        case PLUS: return "+";
        case SEMICOLON: return ";";
        case SLASH: return "/";
        case STAR: return "*";
        case BANG: return "!";
        case BANG_EQUAL: return "!=";
        case EQUAL: return "=";
        case EQUAL_EQUAL: return "==";
        case GREATER: return ">";
        case GREATER_EQUAL: return ">=";
        case LESS: return "<";
        case LESS_EQUAL: return "<=";
        case IDENTIFIER: return "IDENTIFIER";
        case STRING: return "STRING";
        case NUMBER: return "NUMBER";
        case AND: return "and";
        case CLASS: return "class";
        case ELSE: return "else";
        case FALSE: return "false";
        case FN: return "fn";
        case FOR: return "for";
        case IF: return "if";
        case NIL: return "nil";
        case OR: return "or";
        case SAY: return "say";
        case RETURN: return "return";
        case SUPER: return "super";
        case THIS: return "this";
        case TRUE: return "true";
        case LET: return "let";
        case WHILE: return "while";
        case _EOF: return "EOF";
        default: return "Not yet identifed.";
    }
}
