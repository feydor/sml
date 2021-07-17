/* parser.cpp - Recursive descent parser - See GRAMMER.txt for overview */
#include "parser.h"
#include "object.h"
#include <tuple>
#include <iostream>

namespace Parser {

const std::vector<std::unique_ptr<Ast::Stmt>>&
parser::view_ast() const
{
    return ast;
}

// entry point to building AST (abstract syntax tree)
void
parser::scan_program()
{
    while (!at_end()) {
        auto stmt = statement();
        if (stmt != nullptr) // ignore null statements, which are empty statements
            ast.push_back(std::move(stmt));
    }
}

std::unique_ptr<Ast::Stmt>
parser::statement()
{
    // parse user-defined fn and add to FnTable
    if (match(Token::FN)) {
        FnTable::set_fn(userfn());
        return nullptr; // return empty statement, trash it
    }

    if (match(Token::RETURN)) {
        // two types: ret; (implicit return NIL) or ret ident;
        auto default_ret = std::make_unique<Ast::Literal>(std::make_unique<Obj::Number>(0.0));
        if (match(Token::SEMICOLON))
            return std::make_unique<Ast::RetStmt>(std::move(default_ret));
        auto expr = expression();
        match(Token::SEMICOLON); // optional
        return std::make_unique<Ast::RetStmt>(std::move(expr));
    }

    if (match(Token::SAY))
        return std::make_unique<Ast::SayStmt>(expression());

    if (match(Token::IF, Token::ELSE_IF)) {
        consume(Token::LEFT_PAREN, "(", "unexpected character");
        auto cond = expression();
        consume(Token::RIGHT_PAREN, ")", "unexpected character");
        auto body = statement_or_block();

        auto ifstmt = std::make_unique<Ast::IfStmt>(std::move(cond), std::move(body));

        if (match(Token::ELSE))
            ifstmt->add_else(statement_or_block());
        else if (peek_type(Token::ELSE_IF))
            ifstmt->add_else(statement_or_block());
        return ifstmt;
    }

    if (match(Token::WHILE)) {
        consume(Token::LEFT_PAREN, "(", "unexpected character");
        auto cond = expression();
        consume(Token::RIGHT_PAREN, ")", "unexpected character");
        auto body = statement_or_block();
        return std::make_unique<Ast::WhileStmt>(std::move(cond), std::move(body));
    }

    if (match(Token::FOR)) {
        std::unique_ptr<Ast::Stmt> asgmt;
        std::unique_ptr<Ast::Expr> cond;
        std::unique_ptr<Ast::Stmt> control;

        consume(Token::LEFT_PAREN, "(", "unexpected character");

        if (!peek_type(Token::SEMICOLON))
            asgmt = statement();
        match(Token::SEMICOLON);

        if (!peek_type(Token::SEMICOLON))
            cond = expression();
        match(Token::SEMICOLON);

        if (!peek_type(Token::RIGHT_PAREN))
            control = statement();
        consume(Token::RIGHT_PAREN, ")", "unexpected character");
        auto body = statement_or_block();

        // desugar into while loop
        auto final_body = std::make_unique<Ast::BlockStmt>();;
        final_body->add_stmt(std::move(body));
        if (control)
            final_body->add_stmt(std::move(control));

        // if no cond, infinite loop
        if (!cond)
            cond = std::make_unique<Ast::Literal>(std::make_unique<Obj::Bool>(true));

        if (asgmt)
            ast.push_back(std::move(asgmt));
        return std::make_unique<Ast::WhileStmt>(std::move(cond), std::move(final_body));
    }

    if (match(Token::LET)) {
        consume(Token::IDENTIFIER, "an identifier", "unexpected token");
        std::string var = prev().to_str();

        if (match(Token::EQUAL))
            return std::make_unique<Ast::AsgmtStmt>(var, expression());
        else if (match(Token::LEFT_BRACKET))
            return array_decl(var);
        else
            return std::make_unique<Ast::AsgmtStmt>(var,
                std::make_unique<Ast::Literal>(std::make_unique<Obj::Number>(0.0))); // NIL
    }

    // fn_expr
    if (peek_type(Token::IDENTIFIER)) {
        if (peek_next_type(Token::LEFT_PAREN)) {
            auto fncall = std::make_unique<Ast::ExprStmt>(call());
            match(Token::SEMICOLON);
            return fncall;
        }
    }

    if (peek_type(Token::IDENTIFIER)) {
        // var redefinition (asgmt_stmt) or expr_stmt TODO: (var or fn?)
        if (peek_next_type(Token::EQUAL)) {
            match(Token::IDENTIFIER);
            std::string var = prev().to_str();
            match(Token::EQUAL);
            return std::make_unique<Ast::AsgmtStmt>(var, expression());
        } else if (peek_next_type(Token::PLUS_EQUAL) || peek_next_type(Token::MINUS_EQUAL)) {
            match(Token::IDENTIFIER);
            std::string var = prev().to_str();
            match(Token::PLUS_EQUAL, Token::MINUS_EQUAL);
            return std::make_unique<Ast::AsgmtStmt>(var,
                std::make_unique<Ast::Binary>(std::make_unique<Ast::Var>(var),
                    prev().first_subtok(), expression())
            );
        }
        else if (peek_next_type(Token::PLUS_PLUS) || peek_next_type(Token::MINUS_MINUS)) {
            match(Token::IDENTIFIER);
            std::string var = prev().to_str();
            match(Token::PLUS_PLUS, Token::MINUS_MINUS);
            return inc_decrement(var, prev().first_subtok());
        }
        else
            return std::make_unique<Ast::ExprStmt>(expression());
    }

    // if none of the above, then expression statement
    match(Token::SEMICOLON); // optional semicolon
    auto exprstmt = std::make_unique<Ast::ExprStmt>(expression());
    match(Token::SEMICOLON); // optional semicolon
    return exprstmt;
}

std::unique_ptr<Ast::Expr>
parser::expression()
{
    auto expr = ternary();
    match(Token::SEMICOLON); // optional semicolon for all expressions
    return expr;
}

std::unique_ptr<Ast::Expr>
parser::ternary()
{
    auto expr = logical();
    while (match(Token::QUESTION)) {
        Tok op = prev();
        auto iftrue = expression();
        consume(Token::COLON, ":", "unexpected token");
        auto iffalse = expression();
        expr = std::make_unique<Ast::Ternary>(
            std::move(expr), op, std::move(iftrue), std::move(iffalse)
        );
    }
    return expr;
}

std::unique_ptr<Ast::Expr>
parser::logical()
{
    auto expr = equality();
    while (match(Token::OR, Token::AND)) {
        Tok op = prev();
        auto right = equality();
        expr = std::make_unique<Ast::Cond>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Ast::Expr>
parser::equality()
{
    auto expr = comparison();
    while (match(Token::BANG_EQUAL, Token::EQUAL_EQUAL)) {
        Tok op = prev();
        auto right = comparison();
        expr = std::make_unique<Ast::Cond>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Ast::Expr>
parser::comparison()
{
    auto expr = term();
    while (match(Token::GREATER, Token::GREATER_EQUAL,
        Token::LESS, Token::LESS_EQUAL)) {
        Tok op = prev();
        auto right = term();
        expr = std::make_unique<Ast::Cond>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Ast::Expr>
parser::term()
{
    auto expr = factor();
    while (match(Token::MINUS, Token::PLUS)) {
        Tok op = prev();
        auto right = factor();
        expr = std::make_unique<Ast::Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}


std::unique_ptr<Ast::Expr>
parser::factor()
{
    auto expr = unary();
    while (match(Token::SLASH, Token::STAR, Token::PERCENT)) {
        Tok op = prev();
        auto right = unary();
        expr =  std::make_unique<Ast::Binary>(std::move(expr), op, std::move(right));
    }
    return expr;
}

std::unique_ptr<Ast::Expr>
parser::unary()
{
    if (match(Token::BANG, Token::MINUS)) {
        Tok op = prev();
        auto right = unary();
        return std::make_unique<Ast::Unary>(op, std::move(right));
    }
    return call();
}

std::unique_ptr<Ast::Expr>
parser::call()
{
    auto expr = member_access();
    std::string name(prev().to_str()); // TODO: Make sure this is an identifier only

    if (match(Token::LEFT_PAREN)) {
        auto fn_expr = std::make_unique<Ast::FnExpr>(name);
        add_exprs_as_args<Ast::FnExpr>(fn_expr.get());
        return fn_expr;
    }
    return expr;
}

std::unique_ptr<Ast::Expr>
parser::member_access()
{
    auto expr = postfix();
    // std::string object_name(prev().to_str());
    while(match(Token::DOT)) {
        Tok op = prev();
        consume(Token::IDENTIFIER, "an identifier", "unexpected token");
        auto method = std::make_unique<Ast::MethodExpr>(prev().to_str());
        if (match(Token::LEFT_PAREN))
            add_exprs_as_args<Ast::MethodExpr>(method.get());
        expr = std::make_unique<Ast::Binary>(std::move(expr), op, std::move(method));
    }
    return expr;
}

std::unique_ptr<Ast::Expr>
parser::postfix()
{
    auto expr = primary();
    while (match(Token::PLUS_PLUS, Token::MINUS_MINUS)) {
        Tok op = prev();
        expr = std::make_unique<Ast::Unary>(op, std::move(expr));
    }
    return expr;
}

std::unique_ptr<Ast::Expr>
parser::primary()
{
    std::unique_ptr<Ast::Expr> expr;

    if (match(Token::FALSE))
        return std::make_unique<Ast::Literal>(std::make_unique<Obj::Bool>(false));
    if (match(Token::TRUE))
        return std::make_unique<Ast::Literal>(std::make_unique<Obj::Bool>(true));
    if (match(Token::NIL))
        return std::make_unique<Ast::Literal>(std::make_unique<Obj::Number>(0.0));
    if (match(Token::NUMBER))
        return std::make_unique<Ast::Literal>(std::make_unique<Obj::Number>(prev().get_num()));
    if (match(Token::STRING))
        return std::make_unique<Ast::Literal>(std::make_unique<Obj::String>(prev().get_str()));
    if (match(Token::IDENTIFIER)) {
        expr = std::make_unique<Ast::Var>(prev().to_str());
        if (match(Token::LEFT_BRACKET)) {
            Tok op = prev();
            auto right = primary();
            consume(Token::RIGHT_BRACKET, "]", "unexpected character");
            expr = std::make_unique<Ast::Binary>(std::move(expr), op, std::move(right));
        }
        return expr;
    }

    // array primitive
    if (match(Token::LEFT_BRACKET)) {
        int argc = 0;
        std::vector<std::unique_ptr<Ast::Expr>> exprs;
        while (!match(Token::RIGHT_BRACKET)) {
            exprs.push_back(expression());
            match(Token::COMMA);
            if (++argc >= MAXFNARGS)
                throw too_many_args();
        }
        return std::make_unique<Ast::Arr>(std::move(exprs));
    }

    // expression in parens
    if (match(Token::LEFT_PAREN)) {
        expr = expression();
        if (!match(Token::RIGHT_PAREN))
            throw Smol::SyntaxError("unexpected character", ")",
                expr->to_str(), prev().line());
    }

    return expr;
}

std::unique_ptr<Ast::Stmt>
parser::block()
{
    auto block = std::make_unique<Ast::BlockStmt>();
    match(Token::LEFT_BRACE);

    while (!match(Token::RIGHT_BRACE))
        block->add_stmt(statement());

    return block;
}

std::unique_ptr<Ast::Stmt>
parser::statement_or_block()
{
    return peek().type() == Token::LEFT_BRACE
        ? block()
        : statement();
}

// syntactic sugar gen for post/pre incrementer/decrementer
// x++ as x = x + 1 which evals to x+1
std::unique_ptr<Ast::Stmt>
parser::inc_decrement(const std::string &var, const Tok &op)
{
    return std::make_unique<Ast::AsgmtStmt>(var,
        std::make_unique<Ast::Binary>(
            std::make_unique<Ast::Var>(var), op, std::make_unique<Ast::Literal>(std::make_unique<Obj::Number>(1.0))
        )
    );
}

/* array declaration with a size and optional initializer ie let a[100] = {100} */
std::unique_ptr<Ast::Stmt>
parser::array_decl(const std::string& varname)
{
    consume(Token::NUMBER, "a number", "an unexpected token");
    auto size = prev().get_num();
    consume(Token::RIGHT_BRACKET, "]", "unexpected character");

    std::vector<std::shared_ptr<Obj::Object>> values;
    auto val = 0.0;
    if (match(Token::EQUAL)) {
        consume(Token::LEFT_BRACE, "{", "unexpected character");
        consume(Token::NUMBER, "a number", "an unexpected token");
        val = prev().get_num();
        consume(Token::RIGHT_BRACE, "}", "unexpected character");
    }
    for (size_t i = 0; i < size; ++i)
        values.push_back(std::make_shared<Obj::Number>(val));
    
    match(Token::SEMICOLON); // optional semicolon
    return std::make_unique<Ast::AsgmtStmt>(
        varname, std::make_unique<Ast::Literal>(std::make_unique<Obj::Array>(values))
    );
}

std::unique_ptr<FFInterface>
parser::userfn()
{
    consume(Token::IDENTIFIER, "an identifier", "unexpected token");
    auto ident = std::string(prev().to_str());
    consume(Token::LEFT_PAREN, "(", "unexpected character");

    auto userfn = std::make_unique<UserFn>(ident);
    while (!match(Token::RIGHT_PAREN)) {
        consume(Token::IDENTIFIER, "an identifier", "unexpected token");
        userfn->add_argname(prev().to_str());
        match(Token::COMMA);
    }

    userfn->set_body(statement_or_block());
    return userfn;
}

// calls add_arg on fnexpr until match on right parenthesis
// does not claim ownership of pointer
template <typename T>
void
parser::add_exprs_as_args(T* fnexpr)
{
    int argc = 0;
    while (!match(Token::RIGHT_PAREN)) {
        fnexpr->add_arg(expression());
        if (++argc >= MAXFNARGS)
            throw too_many_args();
        match(Token::COMMA);
    }
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
    for (const auto& type : types) {
        if (peek_type(type)) {
            advance();
            return true;
        }
    }
    return false;
}

/* if present, consumes the passed-in token,
 * otherwise, throws a syntax error
 */
void
parser::consume(const Token::type& tok, const std::string& expected, const std::string& msg)
{
    if (!match(tok))
        throw Smol::SyntaxError(msg, expected, peek().to_str(), prev().line());
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

Smol::SyntaxError
parser::too_many_args()
{
    return Smol::SyntaxError("too many arguments", "<= " + std::to_string(MAXFNARGS),
        peek().to_str(), prev().line());
}

}
