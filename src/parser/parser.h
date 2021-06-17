/* a recursive descent parser */
#ifndef PARSER_H
#define PARSER_H
#include "expr.h"
#include "token.h"
#include "stmt.h"
#include <exception>
#include <memory>
#include <vector>

namespace Parser {
class parser {
    std::vector<Token> tokens;
    std::vector<Ast::Stmt *> stmts;
    int curr = 0;

    public:
    parser(std::vector<Token> tokens)
        : tokens(tokens) {};
    std::vector<Ast::Stmt *> scan_program();
    static void error(Token const &tok, Ast::Expr const *curr,
        std::string const &msg);

    private:
    std::vector<Ast::Stmt *> program();
    Ast::Stmt *declaration();
    Ast::Stmt *statement();
    Ast::Stmt *say_stmt();
    Ast::Stmt *expr_stmt();
    Ast::Stmt *block();
    Ast::Stmt *ifstmt();
    Ast::Stmt *elif_stmt();
    Ast::Stmt *else_stmt();
    Ast::Stmt *var_decl();
    Ast::Stmt *var_redef();
    Ast::Expr *expression();
    Ast::Expr *logical();
    Ast::Expr *equality();
    Ast::Expr *comparison();
    Ast::Expr *term();
    Ast::Expr *factor();
    Ast::Expr *unary();
    Ast::Expr *primary();
    
    template <class ...Ts> bool match(Ts... args);
    bool peek_type(Token_t type);
    bool peek_next_type(Token_t type);
    Token peek();
    Token peek_next();
    Token advance();
    Token prev();
    Token consume(Token_t type, Ast::Expr const *curr,
        std::string const& msg);
    bool at_end();
};
}

#endif

