/* parser.cpp - Recursive descent parser - See GRAMMER.txt for overview */
#include "parser.h"
#include "smol.h"
#include <tuple>
#include <iostream>

namespace Parser {
std::vector<Ast::Stmt *>
parser::scan_program()
{
    return program();
}

// entry point to building AST (abstract syntax tree)
std::vector<Ast::Stmt *>
parser::program()
{
    while (!at_end()) {
        Ast::Stmt *stmt = declaration();
        if (stmt)
            this->stmts.push_back(stmt);
    }
    return this->stmts;
}

Ast::Stmt *
parser::declaration()
{
    if (match(LET))
        return var_decl();        
    else if (peek_next_type(EQUAL))
        return var_redef();
    return statement();
}

Ast::Stmt *
parser::statement()
{
    if (match(SAY))
        return say_stmt();
    else if (match(LEFT_BRACE))
        return block();
    else if (match(IF))
        return ifstmt();
    return expr_stmt();
}

Ast::Stmt *
parser::say_stmt()
{
    Ast::Expr *expr = expression();
    consume(EOL, "Expected newline after expression.");
    return new Ast::SayStmt(expr);
}

Ast::Stmt *
parser::expr_stmt()
{
    Ast::Expr *expr = expression();
    consume(EOL, "Expected newline after expression.");
    // expr can be nullptr, if no match
    // for example EOL returns nullptr
    return expr ? new Ast::ExprStmt(expr) : nullptr;
}

Ast::Stmt *
parser::block()
{
    // create a new Env and store in it a list of stmts, like in program()
    // TODO: Mitigate danger of inf loop here when missing closing brace
    match(EOL); // optional linebreak
    Ast::BlockStmt *block = new Ast::BlockStmt();
    while (!match(RIGHT_BRACE)) {
        Ast::Stmt *stmt = declaration();
        if (stmt)
            block->add_stmt(stmt);
    }
    return block;
}

Ast::Stmt *
parser::ifstmt()
{
    Ast::Expr *cond = expression();
    match(EOL); // optional linebreak
    Ast::IfStmt *ifstmt = new Ast::IfStmt(cond, declaration());

    if (peek_type(ELSE_IF)) {
        while (match(ELSE_IF)) {
            ifstmt->add_elif((Ast::ElifStmt *)elif_stmt());
        }
    } else if (match(ELSE)) {
        ifstmt->set_else((Ast::ElseStmt *)else_stmt());
    }
    return ifstmt;
}

Ast::Stmt *
parser::elif_stmt()
{
    Ast::Expr *cond = expression();
    match(EOL); // optional linebreak
    Ast::ElifStmt *elif = new Ast::ElifStmt(cond, declaration());

    if (match(ELSE)) {
        elif->set_else((Ast::ElseStmt *)else_stmt());
    }

    return elif;
}

Ast::Stmt *
parser::else_stmt()
{
    match(EOL); // optional linebreak
    Ast::ElseStmt *else_stmt = new Ast::ElseStmt(declaration());
    return else_stmt;
}

Ast::Stmt *
parser::var_decl() {
    consume(IDENTIFIER, "Expected identifier after keyword 'let'.");
    auto *ident = new Ast::Ident(Sym(Sym_t::VAR, prev().get_lexeme()));
    if (match(EQUAL)) {
        Ast::Expr *expr = expression();
        return new Ast::IdentStmt(ident, expr); // var_def
    }
    return new Ast::IdentStmt(ident); // var_decl
}

Ast::Stmt *
parser::var_redef()
{
    consume(IDENTIFIER, "Expected identifier before token '='.");
    auto *ident = new Ast::Ident(Sym(Sym_t::VAR, prev().get_lexeme()));
    consume(EQUAL,
            "Expected '=' after identifier '" + prev().get_lexeme() + "'");
    Ast::Expr *expr = expression();
    return new Ast::IdentStmt(ident, expr, true); // var_redef
}

Ast::Expr *
parser::expression()
{
    return logical();
}

Ast::Expr *
parser::logical()
{
    Ast::Expr *expr = equality();
    while (match(OR, AND)) {
        Token op = prev();
        Ast::Expr *right = equality();
        expr = new Ast::Binary(expr, op, right);
    }
    return expr;
}

Ast::Expr *
parser::equality()
{
    Ast::Expr *expr = comparison();
    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
        Token op = prev();
        Ast::Expr *right = comparison();
        expr = new Ast::Binary(expr, op, right);
    }
    return expr;
}
Ast::Expr *
parser::comparison()
{
    Ast::Expr *expr = term();
    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        Token op = prev();
        Ast::Expr *right = term();
        expr = new Ast::Binary(expr, op, right);
    }
    return expr;
}

Ast::Expr *
parser::term()
{
    Ast::Expr *expr = factor();
    while (match(MINUS, PLUS)) {
        Token op = prev();
        Ast::Expr *right = factor();
        expr = new Ast::Binary(expr, op, right);
    }
    return expr;
}
Ast::Expr *
parser::factor()
{
    Ast::Expr *expr = unary();
    while (match(SLASH, STAR)) {
        Token op = prev();
        Ast::Expr *right = unary();
        expr =  new Ast::Binary(expr, op, right);
    }
    return expr;
}

Ast::Expr *
parser::unary()
{
    if (match(BANG, MINUS)) {
        Token op = prev();
        Ast::Expr *right = unary();
        return new Ast::Unary(op, right);
    }
    return primary();
}

Ast::Expr *
parser::primary()
{
    if (match(FALSE))
        return new Ast::Literal(Val::Val(false));
    if (match(TRUE))
        return new Ast::Literal(Val::Val(true));
    if (match(NIL))
        return new Ast::Literal(Val::Val());
    if (match(NUMBER))
        return new Ast::Literal(Val::Val(prev().get_literal_num()));
    if (match(STRING))
        return new Ast::Literal(Val::Val(prev().get_literal_str()));
    if (match(IDENTIFIER)) // user-defined identifers
        // TODO: handle function idents
        return new Ast::Ident(Sym(Sym_t::VAR, prev().get_lexeme()));
    if (match(LEFT_PAREN)) {
        Ast::Expr *expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return new Ast::Grouping(expr);
    }
    return nullptr; // curr token: EOL
}

/**
 * checks curr token for type matches
 * consumes and returns true on the first valid match
 */
template <class ...Ts>
bool 
parser::match(Ts... args)
{
    std::vector<Token_t> types {args...};
    for (auto type : types) {
        if (peek_type(type)) {
            advance();
            return true;
        }
    }
    return false;
}

/* checks next token for type match
 * if match, returns true and does not consume */
bool
parser::peek_next_type(Token_t type)
{
    if (at_end())
        return false;
    return peek_next().get_type() == type;
}

/* checks curr token for given type, does not consume it */
bool 
parser::peek_type(Token_t type)
{
    if (at_end())
        return false;
    return peek().get_type() == type;
}  

/* returns curr token, does not consume it */
Token
parser::peek()
{
    return this->tokens.at(this->curr);
}

/* returns next token, does not consume it */
Token
parser::peek_next()
{
    return this->tokens.at(this->curr + 1);
}

/* consumes curr token, returns it, and advances to the next token */
Token
parser::advance()
{
    if (!at_end())
        this->curr++;
    return prev();
}

Token
parser::prev()
{
    return this->tokens.at(this->curr - 1);
}

/* check if next token has type and if so consume, otherwise error */
Token
parser::consume(Token_t type, std::string const& message)
{
    if (peek_type(type))
        return advance();
    parser::error(peek(), message);
    return Token(); // TODO: This should be a nullptr
}
    
bool
parser::at_end()
{
    return peek().get_type() == _EOF;
}

void
parser::error(Token const &tok, std::string const &msg)
{
    if (tok.get_type() == _EOF)
        std::cout << "[line " << tok.get_line() << "] Error "
            << "at end " << msg << std::endl;
    else
        std::cout << "[line " << tok.get_line() << "] Error " << "at '" <<
            tok.get_lexeme() << "' " + msg << std::endl;
}
}
