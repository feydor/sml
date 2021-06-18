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
    std::vector<std::shared_ptr<Ast::Stmt>> stmts;
    int curr = 0;

    public:
    parser(std::vector<Token> tokens)
        : tokens(tokens) {};
    std::vector<std::shared_ptr<Ast::Stmt>> scan_program();
    static void error(Token const &tok, Ast::Expr const *curr,
        std::string const &msg);

    private:
    std::vector<std::shared_ptr<Ast::Stmt>> program();
    std::shared_ptr<Ast::Stmt> declaration();
    std::shared_ptr<Ast::Stmt> statement();
    std::shared_ptr<Ast::Stmt> say_stmt();
    std::shared_ptr<Ast::Stmt> expr_stmt();
    std::shared_ptr<Ast::Stmt> block();
    std::shared_ptr<Ast::Stmt> ifstmt();
    std::shared_ptr<Ast::Stmt> elif_stmt();
    std::shared_ptr<Ast::Stmt> else_stmt();
    std::shared_ptr<Ast::Stmt> var_decl();
    std::shared_ptr<Ast::Stmt> var_redef();
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

