/* a recursive descent parser */
#ifndef PARSER_H
#define PARSER_H
#include <exception>
#include <memory>
#include <stack>
#include <vector>
#include "expr.h"
#include "token.h"
#include "stmt.h"

class Parser {
    std::vector<Token> tokens;
    std::vector<Expr *> exprs;
    std::vector<Stmt *> stmts;
    std::stack<Eval> stack;
    int curr = 0;

    public:
    Parser(std::vector<Token> tokens)
        : tokens(tokens) {};
    std::vector<Expr *> scan_exprs();
    std::vector<Stmt *> scan_program();
    static void error(Token const &tok, std::string const &msg);

    private:
    std::vector<Stmt *> program();
    Stmt * decl();
    Stmt * statement();
    Stmt * say_stmt();
    Stmt * expr_stmt();
    Stmt * var_decl();
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
};

#endif

