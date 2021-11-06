#include <iostream>
#include <memory>
#include "lexer.h"

namespace Lexer {
lexer::lexer(std::string const& src) : src(src)
{
    this->keywords.emplace("def", TokenType::DEF);
    this->keywords.emplace("extern", TokenType::EXTERN);
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
            case '(': add_keyword(std::string(1, c), TokenType::LEFT_PAREN); break;
            case ')': add_keyword(std::string(1, c), TokenType::RIGHT_PAREN); break;
            case '<': add_keyword(std::string(1, c), TokenType::LESS_THAN); break;
            case '+': add_keyword(std::string(1, c), TokenType::PLUS); break;
            case '-': add_keyword(std::string(1, c), TokenType::MINUS); break;
            case '*': add_keyword(std::string(1, c), TokenType::STAR); break;
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
                this->line++;
                break;
            // case '"': str(); break;
            default:
                if (is_digit(c)) {
                    num();
                } else if (is_alpha(c)) {
                    identifier();
                } else {
                    // report unexpected characters
                    // TODO: combine these into a vector and report once
                    add_error("Unexpected character.");
                }
                break;
        }
    }

    this->tokens.emplace_back(Token::make_keyword(TokenType::_EOF, "EOF", this->line));
    return this->tokens;
}

/* consume the current character */
char
lexer::advance()
{
    return this->src.at(this->curr++);
}

// adds a string literal token to the Lexer's tokens
void
lexer::add_string_literal(const std::string &literal) {
    this->tokens.emplace_back(Token::make_string_literal(literal, this->line));
}

// adds a number literal token to the Lexer's tokens
void
lexer::add_number_literal(double num) {
    this->tokens.emplace_back(Token::make_num_literal(num, this->line));
}

void
lexer::add_identifier(const std::string &identifier)
{
    this->tokens.emplace_back(Token::make_identifier(identifier, this->line));
}

// adds a keyword token to the Lexer's tokens
void
lexer::add_keyword(const std::string &keyword, TokenType::type type)
{
    this->tokens.emplace_back(Token::make_keyword(type, keyword, this->line));
}

// attempts to add lexeme as a keyword token
// if not found in keywords, returns false otherwise true
bool
lexer::add_identifier_as_keyword(const std::string &lexeme) {
    auto keyword_itr = this->keywords.find(lexeme);
    if (keyword_itr == this->keywords.end())
        return false;
    
    this->add_keyword(lexeme, keyword_itr->second);
    return true;
}

/* look at current character without consumption */
char
lexer::peek()
{
    if (at_end())
        return '\0';
    return src.at(this->curr);
}

/* lookahead two characters */
char
lexer::double_peek()
{
    if (this->curr + 1 >= (int)src.length())
        return '\0';
    return src.at(this->curr + 1);
}

/* if the next character matches, consume the current + next character */
bool
lexer::next_is(char c)
{
    if (at_end())
        return false;
    if (src.at(this->curr) != c)
        return false;

    this->curr++;
    return true;
}

/* handle string literals */
/*void
lexer::str()
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
    add_token(Token::STRING, literal);
}
*/

void
lexer::num()
{
    // consume consecutive digits
    while (is_digit(peek()))
        advance();

    // check for a single decimal and consume consecutive digits
    if (peek() == '.' && is_digit(double_peek())) {
        advance();
        while (is_digit(peek()))
            advance();
    }

    double num = std::stod(this->src.substr(this->start, this->curr - this->start));
    add_number_literal(num);
}

/* handle non-string letters */
void
lexer::identifier()
{
    // move curr char ptr to end of identifier token
    while (is_alphanumeric(peek()))
        advance();

    std::string lexeme(this->src.substr(this->start, this->curr - this->start));
    
    // search for keywords
    if (add_identifier_as_keyword(lexeme))
        return;
    else
        add_identifier(lexeme);
}

bool
lexer::at_end()
{
    return this->curr >= (int)this->src.length();
}

bool
lexer::is_digit(char c)
{
    return c >= '0' && c <= '9';
}

bool
lexer::is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool
lexer::is_alphanumeric(char c)
{
    return is_alpha(c) || is_digit(c);
}

void
lexer::add_error(const std::string &msg)
{
    this->errors.emplace_back(msg,
        this->src.substr(this->start, this->curr - this->start), this->line);
}

std::vector<Smol::LexerError>
lexer::get_errors()
{
    return this->errors;
}

}
