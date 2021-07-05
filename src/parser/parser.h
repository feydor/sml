/* a recursive descent parser */
#ifndef PARSER_H
#define PARSER_H
#include "expr.h"
#include "token.h"
#include "stmt.h"
#include "smol_error.h"
#include "fntable.h"
#include <exception>
#include <memory>
#include <vector>

namespace Parser {
    class parser {
        std::vector<Tok> tokens;
        std::vector<Ast::Stmt *> stmts;
        int curr = 0;
        static constexpr int MAXFNARGS = 255;

        public:
        explicit parser(std::vector<Tok> tokens)
            : tokens(std::move(tokens)) {};
        std::vector<Ast::Stmt*> scan_program();

        private:
        std::vector<Ast::Stmt*> program();
        Ast::Stmt* statement();
        Ast::Expr* expression();
        Ast::Expr* ternary();
        Ast::Expr* logical();
        Ast::Expr* equality();
        Ast::Expr* comparison();
        Ast::Expr* term();
        Ast::Expr* factor();
        Ast::Expr* unary();
        Ast::Expr* call();
        Ast::Expr* member_access();
        Ast::Expr* primary();

        Ast::Stmt* statement_or_block();
        Ast::Stmt* block();
        Ast::Stmt* inc_decrement(const std::string &var, const Tok &op);
        Ast::Stmt* array_decl(const std::string& varname);
        FFInterface* userfn();
        template <typename T> void add_exprs_as_args(T* fnexpr);
        Smol::SyntaxError too_many_args();

        template <class ...Ts> bool match(Ts... args);
        void consume(const Token::type& tok, const std::string& expected, const std::string& msg);
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

