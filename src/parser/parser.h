#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <map>
#include <string>
#include <exception>
#include "token.h"
#include "prototype.h"
#include "llvm-includes.h"
#include "smol.h"
#include "smol_error.h"

class ExprAST;

class Parser {
    public:
    explicit Parser(const std::vector<Token> &tokens, SMOL &compilerRef)
        : tokens(tokens), compiler(compilerRef) {
        // install the standard binary operators
        // 1 is the lowest precedence
        binary_op_precedences[TokenType::LESS_THAN] = 10;
        binary_op_precedences[TokenType::PLUS]      = 20;
        binary_op_precedences[TokenType::MINUS]     = 30;
        binary_op_precedences[TokenType::STAR]      = 40; // highest
    }

    void parse_syntax();
    std::vector<std::unique_ptr<DeclarationAST>> get_ast();
    void print_ast();

    private:
    std::vector<std::unique_ptr<DeclarationAST>> ast;
    std::vector<Token> tokens;
    SMOL &compiler;
    std::map<TokenType::type, int> binary_op_precedences;
    int curr_token = 0;

    std::unique_ptr<DeclarationAST> parse();
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