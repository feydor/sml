#include <iostream>
#include <memory>
#include "lexer.h"

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
        }
    }

    this->tokens.emplace_back(_EOF, "", this->line);
    return this->tokens;
}

char Lexer::advance()
{
    return this->src.at(this->curr++);
}

void Lexer::add_token(TokenType type)
{
    std::string lexeme(this->src.substr(start, curr));
    this->tokens.emplace_back(type, lexeme, this->line);
}

bool Lexer::at_end()
{
    return this->curr >= (int)this->src.length(); 
}
