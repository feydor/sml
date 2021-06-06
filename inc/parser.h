/* a recursive descent parser */
#ifndef PARSER_H
#define PARSER_H
#include <exception>
#include <memory>
#include <vector>
#include "expr.h"
#include "token.h"

class Parser {
    std::vector<Token> tokens;
    std::vector<Expr *> exprs;
    int curr = 0;

    public:
    Parser(std::vector<Token> tokens)
        : tokens(tokens) {};
    std::vector<Expr *> scan_exprs();

    private:
    Expr *expression();
    Expr *equality();
    Expr *comparison();
    Expr *term();
    Expr *factor();
    Expr *unary();
    Expr *primary();
    
    template <class ...Ts> bool match(Ts... args);
    bool is_type(TokenType type);
    Token peek();
    Token advance();
    Token prev();
    Token consume(TokenType type, std::string const& message);
    bool at_end();
};

#endif

