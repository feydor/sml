#include <iostream>
#include <memory>
#include "lexer.h"
#include "sml.h"

Lexer::Lexer(std::string const& src) 
    : src(src) 
{}

/**
 * For each character of ASCII text in the the source
 * this function detects and adds a token to the tokens vector
 * and returns it.
 */
std::vector<Token> Lexer::scan_tokens()
{
    // a lexeme is made by the substring of [start, curr-start)
    // where curr is ahead of start depending on the char size of the token
    while (!at_end()) {
        this->start = this->curr; // start is reset to curr

        char c = this->advance();
        switch(c) {
            case '(': add_token(LEFT_PAREN); break;
            case ')': add_token(RIGHT_PAREN); break;
            case '{': add_token(LEFT_BRACE); break;
            case '}': add_token(RIGHT_BRACE); break;
            case ',': add_token(COMMA); break;
            case '.': add_token(DOT); break;
            case '-': add_token(MINUS); break;
            case '+': add_token(PLUS); break;
            case ';': add_token(SEMICOLON); break;
            case '*': add_token(STAR); break;
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
            case '/':
                if (next_is('/')) {
                    // comment goes until end of line; skip it
                    // curr ends up pointing to the comment's
                    // new line character
                    while (peek() != '\n' && !at_end())
                        advance();
                } else {
                    add_token(SLASH);
                }
                break;
            case ' ':
            case '\r':
            case '\t':
                // ignore whitespace
                break;
            case '\n':
                this->line++;
                break;
            case '"': this->str(); break;
            default:
                if (is_digit(c)) {
                    this->num();
                } else {
                    // report unexpected characters
                    // TODO: combine these into a vector 
                    // and report as one err
                    SML::error(line, "Unexpected character."); 
                }
                break;
        }
    }

    this->tokens.emplace_back(_EOF, "", this->line);
    return this->tokens;
}

/* consume the current character */
char Lexer::advance()
{
    return this->src.at(this->curr++);
}

void Lexer::add_token(TokenType type)
{
    std::string lexeme(
        this->src.substr(this->start, this->curr - this->start)
    );
    this->tokens.emplace_back(type, lexeme, this->line);
}

/* with literal */
void Lexer::add_token(TokenType type, 
        std::variant<double, std::string> const& literal)
{
    std::string lexeme(
        this->src.substr(this->start, this->curr - this->start)
    );
    this->tokens.emplace_back(type, lexeme, this->line, literal);
}

/* lookahead one character */
char Lexer::peek()
{
    if (at_end()) 
        return '\0';
    return src.at(this->curr);
}

/* lookahead two characters */
char Lexer::peek_next()
{
    if (this->curr + 1 >= (int)src.length()) 
        return '\0';
    return src.at(this->curr + 1);
}

/* if the next character matches, consume the current + next character */
bool Lexer::next_is(char c)
{
    if (at_end())
        return false;
    if (src.at(this->curr) != c)
        return false;

    this->curr++;
    return true;
}

/* handle string literals */
void Lexer::str()
{
    // consume characters until matching " mark
    while (peek() != '"' && !at_end()) {
        if (peek() == '\n') 
            this->line++;
        advance();
    }

    if (at_end()) {
        SML::error(this->line, "Unterminated string.");
        return;
    }

    advance(); // consume the closing " mark

    // trim the quote marks
    std::variant<double, std::string> literal(
        this->src.substr(this->start+1, (this->curr - this->start) - 1)
    );
    add_token(STRING, literal);
}

void Lexer::num()
{
    // consume consequetive digits
    while (is_digit(peek()))
        advance();

    // check for decimal and consume anymore consequetive digits
    if (peek() == '.' && is_digit(peek_next())) {
        advance();
        while (is_digit(peek()))
            advance();
    }

    double num = std::stod(this->src.substr(this->start, this->curr - this->start));
    std::variant<double, std::string> literal(num);
    add_token(NUMBER, literal);
}

bool Lexer::at_end()
{
    return this->curr >= (int)this->src.length(); 
}

bool Lexer::is_digit(char c)
{
    return c >= '0' && c <= '9';
}
