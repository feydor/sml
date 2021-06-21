/* parser.cpp - Recursive descent parser - See GRAMMER.txt for overview */
#include "parser.h"
#include "smol.h"
#include "ansi.h"
#include <tuple>
#include <iostream>
#include <stdexcept>

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
        return new Ast::SayStmt(expression());

    if (match(Token::IF, Token::ELSE_IF)) {
        Ast::Expr* cond = nullptr;
        Ast::Stmt* body = nullptr;
        Ast::IfStmt* ifstmt = nullptr;

        if (!match(Token::LEFT_PAREN))
            throw std::runtime_error("Syntax error: Expected '('.");
        cond = expression();

        if (!match(Token::RIGHT_PAREN))
            throw std::runtime_error("Syntax error: Expected ')'.");
        body = statement_or_block();

        ifstmt = new Ast::IfStmt(cond, body);

        if (match(Token::ELSE))
            ifstmt->add_else(statement_or_block());
        else if (peek_type(Token::ELSE_IF))
            ifstmt->add_else(statement_or_block());
        return ifstmt;
    }
    
    if (match(Token::WHILE)) {
        Ast::Expr* cond = nullptr;
        Ast::Stmt* body = nullptr;

        if (!match(Token::LEFT_PAREN))
            throw std::runtime_error("Syntax error: Expected '('.");
        cond = expression();

        if (!match(Token::RIGHT_PAREN))
            throw std::runtime_error("Syntax error: Expected ')'.");
        body = statement_or_block();

        return new Ast::WhileStmt(cond, body);
    }

    if (match(Token::FOR)) {
        Ast::Stmt* asgmt = nullptr;
        Ast::Expr* cond = nullptr;
        Ast::Stmt* control = nullptr;
        Ast::Stmt* body = nullptr;
        Ast::BlockStmt* final_body = nullptr;

        if (!match(Token::LEFT_PAREN))
            throw std::runtime_error("Syntax error: Expected '('.");
        
        if (!peek_type(Token::SEMICOLON))
            asgmt = statement(); // TODO: var_decl()

        if (!match(Token::SEMICOLON))
            throw std::runtime_error("Syntax error: Expected ';'.");
        else
            cond = expression();

        if (!match(Token::SEMICOLON))
            throw std::runtime_error("Syntax error: Expected ';'.");
        else
            control = statement();

        if (!match(Token::RIGHT_PAREN))
            throw std::runtime_error("Syntax error: Expected ')'.");

        body = statement_or_block();

        // desugar into while loop
        final_body = new Ast::BlockStmt();
        final_body->add_stmt(body);
        if (control)
            final_body->add_stmt(control);

        // if no cond, infinite loop 
        if (!cond)
            cond = new Ast::Literal(Val(true));

        if (asgmt)
            stmts.push_back(asgmt);
        return new Ast::WhileStmt(cond, final_body);
    }

    // TODO: after 'FN', match brace, then body of statements, then closing brace

    if (match(Token::LET)) {
        if (!match(Token::IDENTIFIER))
            throw new std::runtime_error("Syntax error: Expected identifier.");
        std::string var = prev().to_str();

        if (match(Token::EQUAL))
            return new Ast::AsgmtStmt(var, expression());
        else
            return new Ast::AsgmtStmt(var, new Ast::Literal(Val())); // NIL
    }


    if (match(Token::IDENTIFIER)) {
        // var redefinition (asgmt_stmt) or expr_stmt
        std::string var = prev().to_str();
        if (match(Token::EQUAL))
            return new Ast::AsgmtStmt(var, expression());
        else if (match(Token::PLUS_EQUAL, Token::MINUS_EQUAL, Token::STAR_EQUAL,
            Token::SLASH_EQUAL, Token::PERCENT_EQUAL))
            return new Ast::AsgmtStmt(var,
                new Ast::Binary(new Ast::Var(var),
                    prev().first_subtok(), expression())
            );
        else if (match(Token::PLUS_PLUS, Token::MINUS_MINUS))
            return inc_decrement(var, prev().first_subtok());
        else // expr_stmt, a standalone identifier whose evaluation is discarded
            return new Ast::ExprStmt(new Ast::Var(var));
    }

    if (match(Token::LEFT_BRACE)) {
        return block();
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
        Tok op = prev();
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
        Tok op = prev();
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
        Tok op = prev();
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
        Tok op = prev();
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
        Tok op = prev();
        Ast::Expr *right = unary();
        expr =  new Ast::Binary(expr, op, right);
    }
    return expr;
}

Ast::Expr *
parser::unary()
{
    if (match(Token::BANG, Token::MINUS)) {
        Tok op = prev();
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
        if (match(Token::LEFT_PAREN)) {
            // function
            auto fn_expr = new Ast::FnExpr(name);
            while (!match(Token::RIGHT_PAREN)) {
                fn_expr->add_arg(expression());

                // match args-seperating comma, if it exists
                if (peek().type() == Token::COMMA)
                    match(Token::COMMA);
            }

            return fn_expr;
        } else {
            return new Ast::Var(name);
        }
    }

    // expression in parens
    if (match(Token::LEFT_PAREN)) {
        expr = expression();
        if (!match(Token::RIGHT_PAREN))
            throw std::runtime_error("Expected ')' after expression.");
    }

    return expr;
}

Ast::Stmt*
parser::block()
{
    auto block = new Ast::BlockStmt();
    match(Token::LEFT_BRACE);

    while (!match(Token::RIGHT_BRACE))
        block->add_stmt(statement());

    return block;
}

Ast::Stmt*
parser::statement_or_block()
{
    return peek().type() == Token::LEFT_BRACE
        ? block()
        : statement();
}

// syntactic sugar gen for post/pre incrementer/decrementer
// x++ as x = x + 1 which evals to x+1
Ast::Stmt*
parser::inc_decrement(const std::string &var, const Tok &op)
{
    return new Ast::AsgmtStmt(var,
        new Ast::Binary(new Ast::Var(var), op, new Ast::Literal(Val(1.0)))
    );
}

/**
 * checks curr token for type matches
 * consumes and returns true on the first valid match
 */
template <class ...Ts>
bool 
parser::match(Ts... args)
{
    std::vector<Token::type> types {args...};
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
parser::peek_next_type(Token::type type)
{
    if (at_end())
        return false;
    return peek_next().type() == type;
}

/* checks curr token for given type, does not consume it */
bool 
parser::peek_type(Token::type type)
{
    if (at_end())
        return false;
    return peek().type() == type;
}  

/* returns curr token, does not consume it */
Tok
parser::peek()
{
    return this->tokens.at(this->curr);
}

/* returns next token, does not consume it */
Tok
parser::peek_next()
{
    return this->tokens.at(this->curr + 1);
}

/* consumes curr token, returns it, and advances to the next token */
Tok
parser::advance()
{
    if (!at_end())
        this->curr++;
    return prev();
}

Tok
parser::prev()
{
    return this->tokens.at(this->curr - 1);
}
    
bool
parser::at_end()
{
    return peek().type() == Token::_EOF;
}

void
parser::error(Tok const &tok, Ast::Expr const *curr,
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

    std::cout << err << "error" << def << ": " << bold <<
        msg << "\n " << secondary << "--> " << def << fileloc
        << secondary << "\n  |    " << def << expr_str
        << std::endl;
    exit(-1);
}

}
