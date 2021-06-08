#include "parser.h"
#include "smol.h"
#include <tuple>
#include <iostream>

/**
 * a statement is a series of tokens ending with the NEWLINE token
 * for each statement, go through its tokens and combine into
 * expressions and then a statement
 */
std::vector<Expr *>
Parser::scan_exprs() 
{
    while (!at_end()) {
        Expr *e = expression();
        this->exprs.push_back(e);
    } 
    return this->exprs;
}

void
Parser::interpret()
{
    std::cout << "\n begin interpretation...\n";
    for (auto &root : this->exprs) {
        std::cout << "root->op: " + root->op.lexeme << std::endl;
        root->eval(root, this->stack);
        std::cout << "result: " << this->stack.top().val << std::endl;
    }
}

Expr *
Parser::expression()
{
    return equality(); 
}

Expr *
Parser::equality()
{
    Expr *expr = comparison();
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
    if (match(IDENTIFIER))
        return new Expr(prev().lexeme);
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
