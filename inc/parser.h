/* a recursive descent parser */
#ifndef PARSER_H
#define PARSER_H
#include <exception>
#include <memory>
#include <stack>
#include <vector>
#include "expr.h"
#include "token.h"

class Parser {
    std::vector<Token> tokens;
    std::vector<Expr *> exprs;
    std::stack<double> stack;
    int curr = 0;

    public:
    Parser(std::vector<Token> tokens)
        : tokens(tokens) {};
    std::vector<Expr *> scan_exprs();
    void interpret();

    private:
    Expr *expression();
    Expr *equality();
    Expr *comparison();
    Expr *term();
    Expr *factor();
    Expr *unary();
    Expr *primary();
    
    template <class ...Ts> bool match(Ts... args);
    bool peek_type(TokenType type);
    Token peek();
    Token peek_next();
    Token advance();
    Token prev();
    Token consume(TokenType type, std::string const &msg);
    bool at_end();
    static void error(Token const &tok, std::string const &msg);
};

#endif

