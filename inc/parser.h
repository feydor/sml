/* a recursive descent parser */
#ifndef PARSER_H
#define PARSER_H
#include <exception>
#include <vector>
#include "expr.h"
#include "token.h"

class ParseError;

class Parser {
    std::vector<Token> tokens;
    int curr = 0;

    public:
    Parser(std::vector<Token> tokens)
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
    ParseError error();
    bool at_end();
};

class ParseError : public exception {
    virtual const char* what() const throw()
    {
        return "Parse error happened.";
    }
} parse_err;

#endif

