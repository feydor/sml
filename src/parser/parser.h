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
        std::vector<std::unique_ptr<Ast::Stmt>> ast;
        int curr = 0;
        static constexpr int MAXFNARGS = 255;

        public:
        explicit parser(const std::vector<Tok>& tokens) : tokens(tokens) {};
        void scan_program();
        const std::vector<std::unique_ptr<Ast::Stmt>>& view_ast() const;

        private:
        std::unique_ptr<Ast::Stmt> statement();
        std::unique_ptr<Ast::Expr> expression();
        std::unique_ptr<Ast::Expr> ternary();
        std::unique_ptr<Ast::Expr> logical();
        std::unique_ptr<Ast::Expr> equality();
        std::unique_ptr<Ast::Expr> comparison();
        std::unique_ptr<Ast::Expr> term();
        std::unique_ptr<Ast::Expr> factor();
        std::unique_ptr<Ast::Expr> unary();
        std::unique_ptr<Ast::Expr> call();
        std::unique_ptr<Ast::Expr> member_access();
        std::unique_ptr<Ast::Expr> postfix();
        std::unique_ptr<Ast::Expr> primary();

        std::unique_ptr<Ast::Stmt> statement_or_block();
        std::unique_ptr<Ast::Stmt> block();
        std::unique_ptr<Ast::Stmt> inc_decrement(const std::string &var, const Tok &op);
        std::unique_ptr<Ast::Stmt> array_decl(const std::string& varname);
        std::unique_ptr<FFInterface> userfn();
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

