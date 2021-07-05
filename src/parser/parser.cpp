/* parser.cpp - Recursive descent parser - See GRAMMER.txt for overview */
#include "parser.h"
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
        auto stmt = statement();
        if (stmt != nullptr) // ignore null statements, which are empty statements
            stmts.push_back(stmt);
    }
    return stmts;
}

Ast::Stmt *
parser::statement()
{
    // parse user-defined fn and add to FnTable
    if (match(Token::FN)) {
        FnTable::set_fn(userfn());
        return nullptr; // return empty statement, trash it
    }

    if (match(Token::RETURN)) {
        // two types: ret; (implicit return NIL) or ret ident;
        Ast::Literal* def = new Ast::Literal(std::make_shared<Obj::Number>(0.0));
        Ast::Expr* expr = nullptr;
        if (match(Token::SEMICOLON))
            return new Ast::RetStmt(def);
        expr = expression();
        match(Token::SEMICOLON);
        return new Ast::RetStmt(expr);
    }

    if (match(Token::SAY))
        return new Ast::SayStmt(expression());

    if (match(Token::IF, Token::ELSE_IF)) {
        Ast::Expr* cond = nullptr;
        Ast::Stmt* body = nullptr;
        Ast::IfStmt* ifstmt = nullptr;

        consume(Token::LEFT_PAREN, "(", "unexpected character");
        cond = expression();
        consume(Token::RIGHT_PAREN, ")", "unexpected character");
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

        consume(Token::LEFT_PAREN, "(", "unexpected character");
        cond = expression();
        consume(Token::RIGHT_PAREN, ")", "unexpected character");
        body = statement_or_block();
        return new Ast::WhileStmt(cond, body);
    }

    if (match(Token::FOR)) {
        Ast::Stmt* asgmt = nullptr;
        Ast::Expr* cond = nullptr;
        Ast::Stmt* control = nullptr;
        Ast::Stmt* body = nullptr;
        Ast::BlockStmt* final_body = nullptr;

        consume(Token::LEFT_PAREN, "(", "unexpected character");

        if (!peek_type(Token::SEMICOLON))
            asgmt = statement();
        consume(Token::SEMICOLON, ";", "unexpected character");

        if (!peek_type(Token::SEMICOLON))
            cond = expression();
        consume(Token::SEMICOLON, ";", "unexpected character");

        if (!peek_type(Token::RIGHT_PAREN))
            control = statement();
        consume(Token::RIGHT_PAREN, ")", "unexpected character");
        body = statement_or_block();

        // desugar into while loop
        final_body = new Ast::BlockStmt();
        final_body->add_stmt(body);
        if (control)
            final_body->add_stmt(control);

        // if no cond, infinite loop
        if (!cond)
            cond = new Ast::Literal(std::make_shared<Obj::Bool>(true));

        if (asgmt)
            stmts.push_back(asgmt);
        return new Ast::WhileStmt(cond, final_body);
    }

    if (match(Token::LET)) {
        consume(Token::IDENTIFIER, "an identifier", "unexpected token");
        std::string var = prev().to_str();

        if (match(Token::EQUAL))
            return new Ast::AsgmtStmt(var, expression());
        else if (match(Token::LEFT_BRACKET))
            return array_decl(var);
        else
            return new Ast::AsgmtStmt(var, new Ast::Literal(std::make_shared<Obj::Number>(0.0))); // NIL
    }

    // fn_expr
    if (peek_type(Token::IDENTIFIER)) {
        if (peek_next_type(Token::LEFT_PAREN)) {
            return new Ast::ExprStmt(call());
        }
    }

    if (match(Token::IDENTIFIER)) {
        // var redefinition (asgmt_stmt) or expr_stmt TODO: (var or fn?)
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

    // if none of the above, then expression statement
    return new Ast::ExprStmt(expression());
    /*
    throw Smol::SyntaxError("expected a statement", "a statement",
        peek().to_str(), prev().line());
    */
}

Ast::Expr*
parser::expression()
{
    return ternary();
}

Ast::Expr*
parser::ternary()
{
    Ast::Expr* expr = logical();
    while (match(Token::QUESTION)) {
        Tok op = prev();
        Ast::Expr* iftrue = expression();
        consume(Token::COLON, ":", "unexpected token");
        Ast::Expr* iffalse = expression();
        expr = new Ast::Ternary(expr, op, iftrue, iffalse);
    }
    return expr;
}

Ast::Expr*
parser::logical()
{
    Ast::Expr* expr = equality();
    while (match(Token::OR, Token::AND)) {
        Tok op = prev();
        Ast::Expr* right = equality();
        expr = new Ast::Cond(expr, op, right);
    }
    return expr;
}

Ast::Expr*
parser::equality()
{
    Ast::Expr* expr = comparison();
    while (match(Token::BANG_EQUAL, Token::EQUAL_EQUAL)) {
        Tok op = prev();
        Ast::Expr* right = comparison();
        expr = new Ast::Cond(expr, op, right);
    }
    return expr;
}
Ast::Expr*
parser::comparison()
{
    Ast::Expr* expr = term();
    while (match(Token::GREATER, Token::GREATER_EQUAL,
        Token::LESS, Token::LESS_EQUAL)) {
        Tok op = prev();
        Ast::Expr* right = term();
        expr = new Ast::Cond(expr, op, right);
    }
    return expr;
}

Ast::Expr*
parser::term()
{
    Ast::Expr* expr = factor();
    while (match(Token::MINUS, Token::PLUS)) {
        Tok op = prev();
        Ast::Expr* right = factor();
        expr = new Ast::Binary(expr, op, right);
    }
    return expr;
}
Ast::Expr*
parser::factor()
{
    Ast::Expr* expr = unary();
    while (match(Token::SLASH, Token::STAR, Token::PERCENT)) {
        Tok op = prev();
        Ast::Expr* right = unary();
        expr =  new Ast::Binary(expr, op, right);
    }
    return expr;
}

Ast::Expr*
parser::unary()
{
    if (match(Token::BANG, Token::MINUS)) {
        Tok op = prev();
        Ast::Expr* right = unary();
        return new Ast::Unary(op, right);
    }
    return call();
}

Ast::Expr*
parser::call()
{
    Ast::Expr* expr = member_access();
    std::string name(prev().to_str()); // TODO: Make sure this is an identifier only

    if (match(Token::LEFT_PAREN)) {
        auto fn_expr = new Ast::FnExpr(name);
        add_exprs_as_args<Ast::FnExpr>(fn_expr);
        return fn_expr;
    }
    return expr;
}

Ast::Expr*
parser::member_access()
{
    Ast::Expr* expr = primary();
    // std::string object_name(prev().to_str());
    while(match(Token::DOT)) {
        Tok op = prev();
        consume(Token::IDENTIFIER, "an identifier", "unexpected token");
        auto method = new Ast::MethodExpr(prev().to_str());
        if (match(Token::LEFT_PAREN))
            add_exprs_as_args<Ast::MethodExpr>(method);
        expr = new Ast::Binary(expr, op, method);
    }
    return expr;
}

Ast::Expr*
parser::primary()
{
    Ast::Expr* expr = nullptr;

    if (match(Token::FALSE))
        return new Ast::Literal(std::make_shared<Obj::Bool>(false));
    if (match(Token::TRUE))
        return new Ast::Literal(std::make_shared<Obj::Bool>(true));
    if (match(Token::NIL))
        return new Ast::Literal(std::make_shared<Obj::Number>(0.0));
    if (match(Token::NUMBER))
        return new Ast::Literal(std::make_shared<Obj::Number>(prev().get_num()));
    if (match(Token::STRING))
        return new Ast::Literal(std::make_shared<Obj::String>(prev().get_str()));
    if (match(Token::IDENTIFIER)) {
        expr = new Ast::Var(prev().to_str());
        if (match(Token::LEFT_BRACKET)) {
            Tok op = prev();
            Ast::Expr* right = primary();
            consume(Token::RIGHT_BRACKET, "]", "unexpected character");
            expr = new Ast::Binary(expr, op, right);
        }
        return expr;
    }

    // array primitive
    if (match(Token::LEFT_BRACKET)) {
        int argc = 0;
        std::vector<Ast::Expr*> exprs;
        while (!match(Token::RIGHT_BRACKET)) {
            exprs.push_back(expression());
            match(Token::COMMA);
            if (++argc >= MAXFNARGS)
                throw too_many_args();
        }
        return new Ast::Arr(exprs);
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
        new Ast::Binary(new Ast::Var(var), op, new Ast::Literal(std::make_shared<Obj::Number>(1.0)))
    );
}

/* array declaration with a size and optional initializer ie let a[100] = {100} */
Ast::Stmt*
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

    values.resize(size, std::make_shared<Obj::Number>(val));
    return new Ast::AsgmtStmt(varname, new Ast::Literal(std::make_shared<Obj::Array>(values)));
}

FFInterface*
parser::userfn()
{
    consume(Token::IDENTIFIER, "an identifier", "unexpected token");
    std::string ident = std::string(prev().to_str());
    consume(Token::LEFT_PAREN, "(", "unexpected character");

    UserFn* userfn = new UserFn(ident);
    while (!match(Token::RIGHT_PAREN)) {
        consume(Token::IDENTIFIER, "an identifier", "unexpected token");
        userfn->add_argname(prev().to_str());
        match(Token::COMMA);
    }

    userfn->set_body(statement_or_block());
    return userfn;
}

template <typename T>
void
parser::add_exprs_as_args(T* fnexpr)
{
    int argc = 0;
    while (!match(Token::RIGHT_PAREN)) {
        auto param = expression();
        fnexpr->add_arg(param);
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
