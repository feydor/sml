#include <iostream>
#include <memory>
#include "lexer.h"
#include "smol.h"

namespace Lexer {
lexer::lexer(std::string const& src) 
    : src(src) 
{
    this->keywords.emplace("and", AND);
    this->keywords.emplace("else", ELSE);
    this->keywords.emplace("elif", ELSE_IF);
    this->keywords.emplace("false", FALSE);
    this->keywords.emplace("for", FOR);
    this->keywords.emplace("fn", FN);
    this->keywords.emplace("if", IF);
    this->keywords.emplace("nil", NIL);
    this->keywords.emplace("or", OR);
    this->keywords.emplace("say", SAY);
    this->keywords.emplace("ret", RETURN);
    this->keywords.emplace("true", TRUE);
    this->keywords.emplace("let", LET);
    this->keywords.emplace("while", WHILE);
}

/**
 * For each character of ASCII text in the the source
 * this function detects and adds a token to the tokens vector
 * and returns it.
 */
std::vector<Token>
lexer::scan_tokens()
{
    // a lexeme is made by the substring of [start, curr-start)
    // where curr is ahead of start depending on the char size of the token
    while (!at_end()) {
        this->start = this->curr; // start is reset to curr

        char c = advance();
        switch(c) {
            case '(': add_token(LEFT_PAREN); break;
            case ')': add_token(RIGHT_PAREN); break;
            case '{': add_token(LEFT_BRACE); break;
            case '}': add_token(RIGHT_BRACE); break;
            case '[': add_token(LEFT_BRACKET); break;
            case ']': add_token(RIGHT_BRACKET); break;
            case ',': add_token(COMMA); break;
            case '.': add_token(DOT); break;
            case '-': add_token(MINUS); break;
            case '+': add_token(PLUS); break;
            case ';': add_token(SEMICOLON); break;
            case '*': add_token(STAR); break;
            case '%': add_token(PERCENT); break;
            case '/': add_token(SLASH); break;
            case '!':
                add_token(next_is('=') ? BANG_EQUAL : BANG);
                break;
            case '=':
                add_token(next_is('=') ? EQUAL_EQUAL : EQUAL);
                break;
            case '<':
                add_token(next_is('=') ? LESS_EQUAL : LESS);
                break;
            case '>':
                add_token(next_is('=') 
                        ? GREATER_EQUAL : GREATER);
                break;
            case '#':
                // comment goes until end of line; skip it
                while (peek() != '\n' && !at_end())
                    advance();
                // curr char is newline; add it on next iteration
                break;
            case ' ':
            case '\r':
            case '\t':
                // ignore whitespace
                break;
            case '\n':
                add_token(EOL);
                this->line++;
                break;
            case '"': str(); break;
            default:
                if (is_digit(c)) {
                    num();
                } else if (is_alpha(c)) {
                    identifier();
                } else {
                    // report unexpected characters
                    // TODO: combine these into a vector 
                    // and report as one err
                    SMOL::error(line, "Unexpected character."); 
                }
                break;
        }
    }

    this->tokens.emplace_back(_EOF, "", this->line);
    return std::move(this->tokens); // move ownership of smart ptrs
}

/* consume the current character */
char lexer::advance()
{
    return this->src.at(this->curr++);
}

void lexer::add_token(Token_t type)
{
    std::string lexeme(
        this->src.substr(this->start, this->curr - this->start)
    );
    this->tokens.emplace_back(type, lexeme, this->line);
}

/* with literal */
void lexer::add_token(Token_t type, 
        std::variant<double, std::string> const& literal)
{
    std::string lexeme(
        this->src.substr(this->start, this->curr - this->start)
    );
    this->tokens.emplace_back(type, lexeme, this->line, literal);
}

/* look at current character without consumption */
char lexer::peek()
{
    if (at_end()) 
        return '\0';
    return src.at(this->curr);
}

/* lookahead two characters */
char lexer::peek_next()
{
    if (this->curr + 1 >= (int)src.length()) 
        return '\0';
    return src.at(this->curr + 1);
}

/* if the next character matches, consume the current + next character */
bool lexer::next_is(char c)
{
    if (at_end())
        return false;
    if (src.at(this->curr) != c)
        return false;

    this->curr++;
    return true;
}

/* handle string literals */
void lexer::str()
{
    // consume characters until matching " mark
    while (peek() != '"' && !at_end()) {
        if (peek() == '\n') 
            this->line++;
        advance();
    }

    if (at_end()) {
        SMOL::error(this->line, "Unterminated string.");
        return;
    }

    advance(); // consume the closing " mark

    // trim the quote marks and last space
    std::variant<double, std::string> literal(
        this->src.substr(this->start+1, (this->curr - this->start) - 2)
    );
    add_token(STRING, literal);
}

void lexer::num()
{
    // consume consecutive digits
    while (is_digit(peek()))
        advance();

    // check for decimal and consume anymore consecutive digits
    if (peek() == '.' && is_digit(peek_next())) {
        advance();
        while (is_digit(peek()))
            advance();
    }

    double num = std::stod(this->src.substr(this->start, this->curr - this->start));
    std::variant<double, std::string> literal(num);
    add_token(NUMBER, literal);
}

/* handle non-string letters */
void lexer::identifier()
{
    // move curr char ptr to end of identifier token
    while (is_alphanumeric(peek()))
        advance();

    std::string lexeme(
            this->src.substr(this->start, this->curr - this->start)
    );
    auto itr = keywords.find(lexeme);
    itr != keywords.end() ? add_token(itr->second) : add_token(IDENTIFIER);
}

bool lexer::at_end()
{
    return this->curr >= (int)this->src.length(); 
}

bool lexer::is_digit(char c)
{
    return c >= '0' && c <= '9';
}

bool lexer::is_alpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool lexer::is_alphanumeric(char c)
{
    return is_alpha(c) || is_digit(c);
}
}
