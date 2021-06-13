/* a recursive descent parser */
#ifndef PARSER_H
#define PARSER_H
#include "expr.h"
#include "token.h"
#include "stmt.h"
#include <exception>
#include <memory>
#include <vector>

class Parser {
    std::vector<Token> tokens;
    std::vector<Ast::Stmt *> stmts;
    int curr = 0;

    public:
    Parser(std::vector<Token> tokens)
        : tokens(tokens) {};
    std::vector<Ast::Stmt *> scan_program();
    static void error(Token const &tok, std::string const &msg);

    private:
    std::vector<Ast::Stmt *> program();
    Ast::Stmt * decl();
    Ast::Stmt * statement();
    Ast::Stmt * say_stmt();
    Ast::Stmt * expr_stmt();
    Ast::Stmt * var_decl();
    Ast::Expr *expression();
    Ast::Expr *equality();
    Ast::Expr *comparison();
    Ast::Expr *term();
    Ast::Expr *factor();
    Ast::Expr *unary();
    Ast::Expr *primary();
    
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

