#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <string>
#include <exception>
#include "token.h"
#include "expr.h"

class ExprAST;

class Parser {
    public:
    Parser(std::vector<Token> tokens) : tokens(tokens){}
    void parse_syntax();

    private:
    std::vector<std::unique_ptr<ExprAST>> ast;
    std::vector<Token> tokens;
    int curr_token = 0;

    std::unique_ptr<ExprAST> primary();
    std::unique_ptr<ExprAST> identifier();
    std::unique_ptr<ExprAST> number_expr();
    std::unique_ptr<ExprAST> string_expr();
    std::unique_ptr<ExprAST> paren_expr();

    Token peek();
    Token advance();
    bool at_end();
    std::exception throwable_error(const std::string &msg, const std::string &expected,
        const std::string &found, int line);
};

#endif