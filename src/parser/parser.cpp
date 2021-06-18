/* parser.cpp - Recursive descent parser - See GRAMMER.txt for overview */
#include "parser.h"
#include "smol.h"
#include "ansi.h"
#include <tuple>
#include <iostream>

namespace Parser {
std::vector<std::shared_ptr<Ast::Stmt>>
parser::scan_program()
{
    return program();
}

// entry point to building AST (abstract syntax tree)
std::vector<std::shared_ptr<Ast::Stmt>>
parser::program()
{
    while (!at_end()) {
        std::shared_ptr<Ast::Stmt> stmt = declaration();
        if (stmt)
            this->stmts.push_back(stmt);
    }
    return this->stmts;
}

std::shared_ptr<Ast::Stmt>
parser::declaration()
{
    if (match(LET))
        return var_decl();        
    else if (peek_next_type(EQUAL))
        return var_redef();
    return statement();
}

std::shared_ptr<Ast::Stmt>
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

std::shared_ptr<Ast::Stmt>
parser::say_stmt()
{
    Ast::Expr *expr = expression();
    consume(EOL, expr, "say_stmt: Expected newline after expression.");
    return std::make_shared<Ast::SayStmt>(expr);
}

std::shared_ptr<Ast::Stmt>
parser::expr_stmt()
{
    Ast::Expr *expr = expression();
    consume(EOL, expr, "expr_stmt: Expected newline after expression.");
    // expr can be nullptr, if no match
    // for example EOL returns nullptr
    return expr ? std::make_shared<Ast::ExprStmt>(expr) : nullptr;
}

std::shared_ptr<Ast::Stmt>
parser::block()
{
    // create a new Env and store in it a list of stmts, like in program()
    // TODO: Mitigate danger of inf loop here when missing closing brace
    match(EOL); // optional linebreak
    auto block = std::make_shared<Ast::BlockStmt>();
    Ast::BlockStmt *block = new Ast::BlockStmt();
    while (!match(RIGHT_BRACE)) {
        Ast::Stmt *stmt = declaration();
        if (stmt)
            block->add_stmt(stmt);
    }
    return block;
}

std::shared_ptr<Ast::Stmt>
parser::ifstmt()
{
    Ast::Expr *cond = expression();
    match(EOL); // optional linebreak
    auto ifstmt = std::make_shared<Ast::IfStmt>(cond, declaration());
    // Ast::IfStmt *ifstmt = new Ast::IfStmt(cond, declaration());

    if (peek_type(ELSE_IF)) {
        while (match(ELSE_IF)) {
            ifstmt->add_elif(
                dynamic_cast<std::shared_ptr<Ast::ElifStmt>>elif_stmt()
            );
        }
    } else if (match(ELSE)) {
        ifstmt->set_else(
            dynamic_cast<std::shared_ptr<Ast::ElseStmt>>else_stmt()
        );
    }
    return ifstmt;
}

std::shared_ptr<Ast::Stmt>
parser::elif_stmt()
{
    Ast::Expr *cond = expression();
    match(EOL); // optional linebreak
    //Ast::ElifStmt *elif = new Ast::ElifStmt(cond, declaration());
    auto elif = std::make_shared<Ast::ElifStmt>(cond, declaration());

    if (match(ELSE)) {
        elif->set_else(
            dynamic_cast<std::shared_ptr<Ast::ElseStmt>>else_stmt()
        );
    }

    return elif;
}

std::shared_ptr<Ast::Stmt>
parser::else_stmt()
{
    match(EOL); // optional linebreak
    // Ast::ElseStmt *else_stmt = new Ast::ElseStmt(declaration());
    auto else_stmt = std::make_shared<Ast::ElseStmt>(declaration());
    return else_stmt;
}

std::shared_ptr<Ast::Stmt>
parser::var_decl() {
    consume(IDENTIFIER, nullptr, "Expected identifier after keyword 'let'.");
    auto *ident = new Ast::Ident(Sym(Sym_t::VAR, prev().lexeme()));

    if (match(EQUAL)) {
        Ast::Expr *expr = expression();
        return std::make_shared<Ast::IdentStmt>(ident, expr);
        // return new Ast::IdentStmt(ident, expr); // var_def
    }
    return std::make_shared<Ast::IdentStmt>(ident);
    //return new Ast::IdentStmt(ident); // var_decl
}

std::shared_ptr<Ast::Stmt>
parser::var_redef()
{
    consume(IDENTIFIER, nullptr, "Expected identifier before token '='.");
    auto *ident = new Ast::Ident(Sym(Sym_t::VAR, prev().lexeme()));
    consume(EQUAL, ident,
            "Expected '=' after identifier '" + prev().lexeme() + "'");
    Ast::Expr *expr = expression();
    return std::make_shared<Ast::IdentStmt>(ident, expr, true);
    // return new Ast::IdentStmt(ident, expr, true); // var_redef
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
        return new Ast::Ident(Sym(Sym_t::VAR, prev().lexeme()));
    if (match(LEFT_PAREN)) {
        Ast::Expr *expr = expression();
        consume(RIGHT_PAREN, expr, "Expect ')' after expression.");
        return new Ast::Grouping(expr);
    }

    // TODO: fix hacky way of getting around assignment ops
    // evaluates to old lvalue, ignoring the assignment
    if (peek_type(EQUAL)) {
        auto ident = new Ast::Ident(Sym(Sym_t::VAR, prev().lexeme()));
        advance();
        advance();
        return ident;
    }
    /*
    if (peek_type(EOL))
        std::cout << "match EOL\n";
    else 
        std::cout << "match nothing " << prev().to_str() <<
        " " << peek().to_str() << " " << peek_next().to_str() << "\n";
    */
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

/* check if next token has type and if so consume, otherwise error */
Token
parser::consume(Token_t type, Ast::Expr const *curr,
    std::string const& msg)
{
    if (peek_type(type))
        return advance();
    parser::error(peek(), curr, msg);

    // unreached
    return Token();
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
