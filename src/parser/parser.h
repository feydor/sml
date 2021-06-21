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
    std::vector<Tok> tokens;
    std::vector<Ast::Stmt *> stmts;
    int curr = 0;

    public:
    parser(std::vector<Tok> tokens)
        : tokens(std::move(tokens)) {};
    std::vector<Ast::Stmt*> scan_program();
    static void error(Tok const &tok, Ast::Expr const *curr,
        std::string const &msg);

    private:
    std::vector<Ast::Stmt*> program();
    Ast::Stmt* statement();
    Ast::Expr* expression();
    Ast::Expr* logical();
    Ast::Expr* equality();
    Ast::Expr* comparison();
    Ast::Expr* term();
    Ast::Expr* factor();
    Ast::Expr* unary();
    Ast::Expr* primary();

    Ast::Stmt* statement_or_block();
    Ast::Stmt* block();
    Ast::Stmt* inc_decrement(const std::string &var, const Tok &op);
    
    template <class ...Ts> bool match(Ts... args);
    bool peek_type(Token::type type);
    bool peek_next_type(Token::type type);
    Tok peek();
    Tok peek_next();
    Tok advance();
    Tok prev();
    bool at_end();
};
}

#endif

