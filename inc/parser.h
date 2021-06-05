/* a recursive descent parser */
#ifndef PARSER_H
#define PARSER_H
#include <exception>
#include <vector>
#include "expr.h"
#include "token.h"

class Parser {
    std::vector<std::unique_ptr<Token>> tokens;
    int curr = 0;

    public:
    Parser(std::vector<std::unique_ptr<Token>> tokens)
        : tokens(tokens) {};

    private:
    Expr &expression();
    Expr &equality();
    Expr &comparison();
    Expr &term();
    Expr &factor();
    Expr &unary();
    Expr &primary();
    
    template <class... Args> bool match(Args&&... args);
    bool is_type(TokenType type);
    Token &peek();
    Token &advance();
    Token &prev();
    Token &consume();
    bool at_end();
};

#endif

