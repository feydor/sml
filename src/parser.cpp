#include "parser.h"
#include "smol.h"
#include <tuple>
#include <iostream>

std::vector<Expr *>
Parser::scan_exprs() 
{
    std::cout << "begin scan_exprs" << std::endl;
    while (!at_end()) {
        Expr *expr = expression();
        this->exprs.push_back(expr);
    } 
    std::cout << "end scan_exprs" << std::endl;
    return this->exprs;
}

/*
bool
Parser::accept(Symbol sym)
{
    if (sym == this.tokens.at(this->curr)->type) {
        advance();
        return true;
    }
    return false;
}

bool Parser::expect(Symbol sym)
{
    return accept(sym) ? true : false;
}
*/

/**
 * begin recursive descent parser
 * see GRAMMER.md for high-level routine outline
 */
Expr *
Parser::expression() 
{
    return equality();
}

Expr *
Parser::equality()
{
    Expr *expr = comparison();

    // (...)*
    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
        Token op = prev();
        Expr *right = comparison();
        return new Expr(expr, op, right);
    }
    return expr;
}

Expr *
Parser::comparison()
{
    Expr *expr = term();
    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        Token op = prev();
        Expr *right = term();
        return new Expr(expr, op, right);
    }
    return expr;
}

Expr *
Parser::term()
{
    Expr *expr = factor();
    while (match(MINUS, PLUS)) {
        Token op = prev();
        Expr *right = factor();
        return new Expr(expr, op, right);
    }
    return expr;
}

Expr *
Parser::factor()
{
    Expr *expr = unary();
    while (match(SLASH, STAR)) {
        Token op = prev();
        Expr *right = unary();
        return new Expr(expr, op, right);
    }
    return expr;
}

Expr *
Parser::unary()
{
    if (match(BANG, MINUS)) {
        Token op = prev();
        Expr *right = unary();
        return new Expr(op, right);
    }
    return primary();
}

Expr *
Parser::primary()
{
    if (match(FALSE))
        return new Expr(false);
    if (match(TRUE))
        return new Expr(true);
    if (match(NIL))
        return new Expr("nullptr");
    if (match(NUMBER))
        return new Expr(std::get<double>(prev().literal));
    if (match(STRING))
        return new Expr(std::get<std::string>(prev().literal));
    if (match(LEFT_PAREN)) {
        Expr *expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return new Expr(expr);
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
    // std::cout << Token::type_to_string(types[0]) << std::endl;
    for (auto type : types) {
        if (is_type(type)) {
            advance();
            return true;
        }
    }
    return false;
}

/* checks curr token for given type, does not consume it */
bool 
Parser::is_type(TokenType type)
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
    if (is_type(type))
        return advance();
    SMOL::error(peek(), message);
    return Token(); // TODO: This should be a nullptr
}
    
bool
Parser::at_end()
{
    return peek().type == _EOF;
}
