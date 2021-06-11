/* parser.cpp - Recursive descent parser - See GRAMMER.txt for overview */
#include "parser.h"
#include "smol.h"
#include <tuple>
#include <iostream>

/**
 * a statement is a series of tokens ending with the NEWLINE token
 * for each statement, go through its tokens and combine into
 * expressions and then a statement
 */
std::vector<Ast::Expr *>
Parser::scan_exprs() 
{
    while (!at_end()) {
        Ast::Expr *e = expression();
        this->exprs.push_back(e);
    } 
    return this->exprs;
}

std::vector<Ast::Stmt *>
Parser::scan_program()
{
    return program();
}

// entry point to building AST (abstract syntax tree)
std::vector<Ast::Stmt *>
Parser::program()
{
    while (!at_end()) {
        Ast::Stmt *stmt = decl();
        if (stmt)
            this->stmts.push_back(stmt);
    }
    return this->stmts;
}

Ast::Stmt *
Parser::decl()
{
    if (match(LET))
        return var_decl();        
    return statement();
}

Ast::Stmt *
Parser::statement()
{
    if (match(SAY))
        return say_stmt();
    return expr_stmt();
}

Ast::Stmt *
Parser::say_stmt()
{
    Ast::Expr *say_keyword = new Ast::Expr(prev().lexeme, prev().type);
    Ast::Expr *expr = expression();
    consume(EOL, "Expected newline after expression.");
    return new Ast::Stmt(Stmt_t::SAY, say_keyword, expr);
}

Ast::Stmt *
Parser::expr_stmt()
{
    Ast::Expr *expr = expression();
    consume(EOL, "Expected newline after expression.");
    // expr can be nullptr, if no match
    // for example EOL returns nullptr
    if (expr)
        return new Ast::Stmt(Stmt_t::EXPR, expr, true); // true is placeholder
    else
        return nullptr;
}

Ast::Stmt *
Parser::var_decl() {
    consume(IDENTIFIER, "Expected identifier after keyword 'let'.");
    Ast::Expr *ident = new Ast::Expr(prev().lexeme);
    if (match(EQUAL)) {
        Ast::Expr *expr = expression();
        return new Ast::Stmt(Stmt_t::VAR_DECL, ident, expr);
    }
    return new Ast::Stmt(Stmt_t::VAR_DECL, ident);
}

Ast::Expr *
Parser::expression()
{
    return equality(); 
}

Ast::Expr *
Parser::equality()
{
    Ast::Expr *expr = comparison();
    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
        Token op = prev();
        Ast::Expr *right = comparison();
        expr = new Ast::Expr(expr, op, right);
    }
    return expr;
}
Ast::Expr *
Parser::comparison()
{
    Ast::Expr *expr = term();
    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        Token op = prev();
        Ast::Expr *right = term();
        expr = new Ast::Expr(expr, op, right);
    }
    return expr;
}

Ast::Expr *
Parser::term()
{
    Ast::Expr *expr = factor();
    while (match(MINUS, PLUS)) {
        Token op = prev();
        Ast::Expr *right = factor();
        expr = new Ast::Expr(expr, op, right);
    }
    return expr;
}
Ast::Expr *
Parser::factor()
{
    Ast::Expr *expr = unary();
    while (match(SLASH, STAR)) {
        Token op = prev();
        Ast::Expr *right = unary();
        expr =  new Ast::Expr(expr, op, right);
    }
    return expr;
}

Ast::Expr *
Parser::unary()
{
    if (match(BANG, MINUS)) {
        Token op = prev();
        Ast::Expr *right = unary();
        return new Ast::Expr(op, right);
    }
    return primary();
}

Ast::Expr *
Parser::primary()
{
    if (match(FALSE))
        return new Ast::Expr(false);
    if (match(TRUE))
        return new Ast::Expr(true);
    if (match(NIL))
        return new Ast::Expr("nullptr");
    if (match(NUMBER))
        return new Ast::Expr(std::get<double>(prev().literal));
    if (match(STRING))
        return new Ast::Expr(std::get<std::string>(prev().literal));
    if (match(IDENTIFIER)) // user-defined identifers
        return new Ast::Expr(prev().lexeme, Expr_t::LITERAL);
    if (match(LEFT_PAREN)) {
        Ast::Expr *expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return new Ast::Expr(expr);
    }
    return nullptr;
}

/**
 * checks curr token for type matches
 * consumes and returns true on the first valid match
 */
template <class ...Ts>
bool 
Parser::match(Ts... args)
{
    std::vector<TokenType> types {args...};
    for (auto type : types) {
        if (peek_type(type)) {
            advance();
            return true;
        }
    }
    return false;
}

/* checks curr token for given type, does not consume it */
bool 
Parser::peek_type(TokenType type)
{
    if (at_end())
        return false;
    return peek().type == type;
}  

/* returns curr token, does not consume it */
Token
Parser::peek()
{
    return this->tokens.at(this->curr);
}

/* returns next token, does not consume it */
Token
Parser::peek_next()
{
    return this->tokens.at(this->curr + 1);
}

/* consumes curr token, returns it, and advances to the next token */
Token
Parser::advance()
{
    if (!at_end())
        this->curr++;
    return prev();
}

Token
Parser::prev()
{
    return this->tokens.at(this->curr - 1);
}

/* check if next token has type and if so consume, otherwise error */
Token
Parser::consume(TokenType type, std::string const& message)
{
    if (peek_type(type))
        return advance();
    Parser::error(peek(), message);
    return Token(); // TODO: This should be a nullptr
}
    
bool
Parser::at_end()
{
    return peek().type == _EOF;
}

void
Parser::error(Token const &tok, std::string const &msg)
{
    if (tok.type == _EOF)
        std::cout << "[line " << tok.line << "] Error " << "at end " <<
            msg << std::endl;
    else
        std::cout << "[line " << tok.line << "] Error " << "at '" <<
            tok.lexeme << "' " + msg << std::endl;
}
