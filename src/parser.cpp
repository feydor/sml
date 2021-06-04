#include "parser.h"
#include <memory>
#include <tuple>

Expr &
Parser::expression() 
{
    return equality();
}

Expr &
Parser::equality()
{
    Expr &expr = comparison();

    // (...)*
    while (match(BANG_EQUAL, EQUAL_EQUAL)) {
        Token &op = prev(); // get just matched token
        Expr &right = comparison();
        // expr = new Binary e(expr, op, right);
    }
    return expr;
}

Expr &
Parser::comparison()
{
    Expr &expr = term();
    while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        Token &op = prev();
        Expr &right = term();
        // expr = new Binary e(expr, op, right);
    }
    return expr;
}

Expr &
Parser::term()
{
    Expr &expr = factor();
    while (match(MINUS, PLUS)) {
        Token &op = prev();
        Expr &right = factor();
        // expr = new Binary e(expr, op, right);
    }
    return expr;
}

Expr &
Parser::factor()
{
    Expr &expr = unary();
    while (match(MINUS, PLUS)) {
        Token &op = prev();
        Expr &right = unary();
        // expr = new Binary e(expr, op, right);
    }
    return expr;
}

Expr &
Parser::unary()
{
    if (match(BANG, MINUS)) {
        Token &op = prev();
        Expr &right = unary();
        // return std::unique_ptr<Unary> ex(new Unary(op, right));
    }
    return primary();
}

Expr &
Parser::primary()
{
    if (match(FALSE))
        return std::unique_ptr<Literal> ex(new Literal(false));
    if (match(TRUE))
        return std::unique_ptr<Literal> ex(new Literal(true));
    if (match(NIL))
        return std::unique_ptr<Literal> ex(new Literal(nullptr));

    if (match(NUMBER, STRING))
        return std::unique_ptr<Literal> ex(new Literal(prev().literal));

    if (match(LEFT_PAREN)) {
        Expr &expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return std::unique_ptr<Grouping> ex(new Grouping(expr));
    }
}

/**
 * checks curr token for type matches
 * consumes and returns true on the first valid match
 */
template <class... Args>
bool 
Parser::match(Args&&... args)
{
    auto types = std::make_tuple(std::forward<Args>(args)...);

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
    return peek()->type == type;
}  

/* returns curr token, does not consume it */
Token &
Parser::peek()
{
    return this->tokens.at(this->curr);
}

/* consumes curr token, returns it, and advances to the next token */
Token &
Parser::advance()
{
    if (!at_end())
        this->cur++;
    return prev();
}

Token &
Parser::prev()
{
    return this->tokens.at(this->curr - 1);
}

/* check if next token has type and if so consume, otherwise error */
Token &
Parser::consume(TokenType type, std::string const& message)
{
    if (is_type(type))
        return advance();
    throw Parser::error(peek(), message);
}
    
ParseError
error(Token &tok, std::string const& message)
{
    SMOL::error(tok, message);
    return parse_err;
}

bool
Parser::at_end()
{
    return peek.type() == _EOF;
}
