/* parser.cpp - Recursive descent parser - See GRAMMER.txt for overview */
#include "parser.h"
#include "smol.h"
#include "ansi.h"
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
        stmts.push_back(statement());
    }
    return stmts;
}

Ast::Stmt *
parser::statement()
{
    if (match(Token::SAY))
        return new SayStmt(expression());

    if (match(Token::IF)) {
        Ast::Expr* cond = nullptr;
        Ast::Stmt* ifstmt = nullptr;
        Ast::Stmt* elsestmt = nullptr;

        if (!match(Token::LEFT_PAREN))
            throw std::runtime_error("Syntax error: Expected '('.");
        cond() = expression();

        if (!match(Token::RIGHT_PAREN))
            throw std::runtime_error("Syntax error: Expected ')'.");
        ifstmt = statement();

        if (match(Token::ELSE))
            elsestmt = statement();
        return new Ast::IfStmt(cond, ifstmt, elsestmt);
    }

    /*
    if (match(Token::WHILE)) {

    }
    if (match(Token::FOR)) {
    
    }
    */

    if (match(Token::LEFT_BRACE)) {
        auto blockstmt = new Ast::BlockStmt();
        while (!match(Token::RIGHT_BRACE))
            blockstmt->add_stmt(statement());
        return blockstmt;
    }

    if (match(Token::LET)) {
        if (!match(Token::IDENTIFIER))
            throw new std::runtime_error("Syntax error: Expected identifier.");
        std::string var = prev().to_str();

        if (match(Token::EQUAL))
            return new Ast::AsgmtExpr(var, expression());
    }

    // if none of the above, then expression statement
    return new Ast::ExprStmt(expression());
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
    while (match(Token::OR, Token::AND)) {
        Token op = prev();
        Ast::Expr *right = equality();
        expr = new Ast::Cond(expr, op, right);
    }
    return expr;
}

Ast::Expr *
parser::equality()
{
    Ast::Expr *expr = comparison();
    while (match(Token::BANG_EQUAL, Token::EQUAL_EQUAL)) {
        Token op = prev();
        Ast::Expr *right = comparison();
        expr = new Ast::Cond(expr, op, right);
    }
    return expr;
}
Ast::Expr *
parser::comparison()
{
    Ast::Expr *expr = term();
    while (match(Token::GREATER, Token::GREATER_EQUAL,
        Token::LESS, Token::LESS_EQUAL)) {
        Token op = prev();
        Ast::Expr *right = term();
        expr = new Ast::Cond(expr, op, right);
    }
    return expr;
}

Ast::Expr *
parser::term()
{
    Ast::Expr *expr = factor();
    while (match(Token::MINUS, Token::PLUS)) {
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
    while (match(Token::SLASH, Token::STAR, Token::PERCENT)) {
        Token op = prev();
        Ast::Expr *right = unary();
        expr =  new Ast::Binary(expr, op, right);
    }
    return expr;
}

Ast::Expr *
parser::unary()
{
    if (match(Token::BANG, Token::MINUS)) {
        Token op = prev();
        Ast::Expr *right = unary();
        return new Ast::Unary(op, right);
    }
    return primary();
}

Ast::Expr *
parser::primary()
{
    Ast::Expr* expr = nullptr;

    if (match(Token::FALSE))
        return new Ast::Literal(Val(false));
    if (match(Token::TRUE))
        return new Ast::Literal(Val(true));
    if (match(Token::NIL))
        return new Ast::Literal(Val());
    if (match(Token::NUMBER))
        return new Ast::Literal(Val(prev().get_num()));
    if (match(Token::STRING))
        return new Ast::Literal(Val(prev().get_str()));

    if (match(Token::IDENTIFIER)) {
        // function or variable
        std::string name(prev().to_str());
        if (match(TOKEN::LEFT_PAREN)) {
            // function
            auto fn_expr = Ast::Fn(name);
            while (!match(Token::RIGHT_PAREN)) {
                fn_expr->add_arg(expression());

                // match args-seperating comma, if it exists
                if (peek().type() == Token::COMMA)
                    match(Token::COMMA);
            }

            // TODO: match brace, then body of statements, then closing brace
            return fn_expr;
        } else {
            // variable
            return new Ast::Var(name);
        }
    }

    // expresssion in parenthesis
    if (match(Token::LEFT_PAREN)) {
        expr = expression();
        if (!match(RIGHT_PAREN))
            throw std::runtime_error("Expected ')' after expression.");
    }

    return expr;
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
    return peek_next().type() == type;
}

/* checks curr token for given type, does not consume it */
bool 
parser::peek_type(Token_t type)
{
    if (at_end())
        return false;
    return peek().type() == type;
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
    
bool
parser::at_end()
{
    return peek().type() == _EOF;
}

void
parser::error(Token const &tok, Ast::Expr const *curr,
    std::string const &msg)
{
    ANSI::Modifier err(Color::FG_RED);
    ANSI::Modifier secondary(Color::FG_BLUE);
    ANSI::Modifier bold(Format::BOLD);
    ANSI::Modifier def(Color::FG_DEFAULT);

    std::string fileloc("");
    std::string expr_str("");

    if (!SMOL::is_repl) // add tok.offset()
        fileloc += SMOL::fname + ":" + std::to_string(tok.line());

    if (!curr) {
        std::cout << err << "error" << def << ": " << bold <<
            msg << "\n " << secondary << "--> " << def << fileloc
            << std::endl;
        exit(-1);
    }

    if (curr->is_ident())
        expr_str = ((Ast::Ident *)curr)->to_str();
    else if (curr->is_literal())
        expr_str = ((Ast::Literal *)curr)->to_str();
    else if (curr->is_binary())
        expr_str = ((Ast::Binary *)curr)->to_str();
    else if (curr->is_unary())
        expr_str = ((Ast::Unary *)curr)->to_str();

    std::cout << err << "error" << def << ": " << bold <<
        msg << "\n " << secondary << "--> " << def << fileloc
        << secondary << "\n  |    " << def << expr_str
        << std::endl;
    exit(-1);
}

}
