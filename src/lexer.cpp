#include <iostream>
#include <memory>
#include "lexer.h"
#include "sml.h"

Lexer::Lexer(std::string const& src) 
    : src(src) 
{}

std::vector<Token> Lexer::scan_tokens()
{
    while (!this->at_end()) {
        this->start = this->curr;

        char c = this->advance();
        switch(c) {
            case '(': this->add_token(LEFT_PAREN); break;
            case ')': this->add_token(RIGHT_PAREN); break;
            case '{': this->add_token(LEFT_BRACE); break;
            case '}': this->add_token(RIGHT_BRACE); break;
            case ',': this->add_token(COMMA); break;
            case '.': this->add_token(DOT); break;
            case '-': this->add_token(MINUS); break;
            case '+': this->add_token(PLUS); break;
            case ';': this->add_token(SEMICOLON); break;
            case '*': this->add_token(STAR); break;
            case '!':
                this->add_token(this->next_is('=') ? BANG_EQUAL : BANG);
                break;
            case '=':
                this->add_token(this->next_is('=') ? EQUAL_EQUAL : EQUAL);
                break;
            case '<':
                this->add_token(this->next_is('=') ? LESS_EQUAL : LESS);
                break;
            case '>':
                this->add_token(this->next_is('=') 
                        ? GREATER_EQUAL : GREATER);
                break;
            case '/':
                if (this->next_is('/')) {
                    // comment goes until end of line
                    while (this->peek() != '\n' && !this->at_end())
                        this->advance();
                } else {
                    this->add_token(SLASH);
                }
                break;
            default:
                // report unexpected characters
                // TODO: combine these into a vector and report as one err
                SML::error(line, "Unexpected character."); 
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
    // lexeme = src[start_index, end_index) where end_index = curr - start
    std::string lexeme(this->src.substr(start, curr - start));
    this->tokens.emplace_back(type, lexeme, this->line);
}

/* lookahead one character */
char Lexer::peek()
{
    if (this->at_end()) 
        return '\0';
    return src.at(this->curr);
}

/* if the next character matches, consume the current + next character */
bool Lexer::next_is(char c)
{
    if (this->at_end())
        return false;
    if (src.at(this->curr) != c)
        return false;

    this->curr++;
    return true;
}

bool Lexer::at_end()
{
    return this->curr >= (int)this->src.length(); 
}
