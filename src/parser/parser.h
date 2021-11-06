#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <map>
#include <string>
#include <exception>
#include "token.h"
#include "expr.h"
#include "prototype.h"
#include "smol_error.h"

class ExprAST;

class Parser {
    public:
    Parser(std::vector<Token> tokens) : tokens(tokens){
        // install the standard binary operators
        // 1 is the lowest precedence
        binary_op_precedences[TokenType::LESS_THAN] = 10;
        binary_op_precedences[TokenType::PLUS]      = 20;
        binary_op_precedences[TokenType::MINUS]     = 30;
        binary_op_precedences[TokenType::STAR]      = 40; // highest
    }

    void parse_syntax();
    void print_ast();

    private:
    std::vector<std::unique_ptr<FunctionAST>> ast;
    std::vector<Token> tokens;
    std::map<TokenType::type, int> binary_op_precedences;
    int curr_token = 0;

    std::unique_ptr<FunctionAST> parse();
    std::unique_ptr<FunctionAST> function_definition();
    std::unique_ptr<FunctionAST> toplevel_expr();
    std::unique_ptr<ExprAST> expression();
    std::unique_ptr<ExprAST> binary_rhs(int precedence, std::unique_ptr<ExprAST> LHS);
    std::unique_ptr<ExprAST> primary();
    std::unique_ptr<ExprAST> identifier();
    std::unique_ptr<ExprAST> number_expr();
    std::unique_ptr<ExprAST> paren_expr();
    std::unique_ptr<PrototypeAST> prototype();
    std::unique_ptr<PrototypeAST> extern_definition();

    Token peek();
    Token advance();
    bool at_end();
    int token_precedence(Token token);
    bool is_binary_op(Token token);
    Smol::ParserError throwable_error(const std::string &msg, const std::string &expected,
        const std::string &found, int line);
};

#endif