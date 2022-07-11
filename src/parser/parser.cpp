#include "parser.h"
#include "expr.h"
#include <cassert>
#include <iostream>

namespace ASTPrinter {
    std::string to_str(DeclarationAST &fn_expr)
    {
        std::string out("");
        out += fn_expr.get_name() + " ";
        for (auto arg : fn_expr.get_args())
            out += arg + " ";
        return out;
    }
}

std::vector<std::unique_ptr<DeclarationAST>>
Parser::get_ast()
{
    return std::move(ast);
}

void
Parser::parse_syntax()
{   
    while (!at_end()) {
        if (auto expr = parse())
            ast.push_back(std::move(expr));
    }
}

std::unique_ptr<DeclarationAST>
Parser::parse()
{
    switch(peek().get_type()) {
        case TokenType::DEF: return function_definition();
        case TokenType::EXTERN: return extern_definition();
        default: return toplevel_expr();
    }
    return nullptr;
}

// function => 'def' prototype expression
std::unique_ptr<FunctionAST>
Parser::function_definition()
{
    advance(); // consume 'def'

    auto proto = prototype();
    if (!proto) return nullptr;

    if (auto expr = expression())
        return std::make_unique<FunctionAST>(std::move(proto), std::move(expr));
    return nullptr;
}

// toplevelexpr => expression
std::unique_ptr<FunctionAST>
Parser::toplevel_expr()
{
    if (auto expr = expression()) {
        auto anonymous_proto = std::make_unique<PrototypeAST>("__anon_expr", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(anonymous_proto), std::move(expr));
    }
    return nullptr;
}

// prototype => identifier '(' identifier* ')'
std::unique_ptr<PrototypeAST>
Parser::prototype()
{
    auto ident_token = advance();

    if (peek().get_type() != TokenType::LEFT_PAREN)
        throw throwable_error("Syntax error in prototype", "(", peek().get_lexeme(),
                                                                peek().get_line());
    advance(); // consume '('
    std::vector<std::string> arg_names;
    while (peek().get_type() != TokenType::RIGHT_PAREN)
        arg_names.push_back(advance().get_identifier());

    advance(); // consume ')'
    return std::make_unique<PrototypeAST>(ident_token.get_identifier(), std::move(arg_names));
}

// extern => 'extern' prototype
std::unique_ptr<PrototypeAST>
Parser::extern_definition()
{
    advance(); // consume 'extern'
    return prototype();
}

// expression => primary binary_rhs
std::unique_ptr<ExprAST>
Parser::expression()
{
    auto LHS = primary();
    if (!LHS)
        return nullptr;
    return binary_rhs(0, std::move(LHS));
}

// binary_rhs => ('+' primary)*
std::unique_ptr<ExprAST>
Parser::binary_rhs(int precedence, std::unique_ptr<ExprAST> LHS)
{
    while (1) {
        int tok_prec = token_precedence(peek());

        // not a binary operator or same/lower precedence
        if (tok_prec < precedence)
            return LHS;
        
        // if this is a binary operator, find its precedence
        assert(is_binary_op(peek()));
        
        auto binary_op_token = advance(); // consume operator
        auto RHS = primary();
        if (!RHS)
            return nullptr;
        
        // determine associativity of next binary op
        int next_prec = token_precedence(peek());
        
        // 
        if (tok_prec < next_prec) {
            RHS = binary_rhs(tok_prec + 1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        // merge LHS into RHS to make complete binary expr
        LHS = std::make_unique<BinaryExprAST>(binary_op_token.get_lexeme(), std::move(LHS),
                                                                            std::move(RHS));
    }
}

std::unique_ptr<ExprAST>
Parser::primary()
{
    switch (peek().get_type()) {
        case TokenType::IDENTIFIER:
            return identifier();
        case TokenType::NUMBER:
            return number_expr();
        case TokenType::LEFT_PAREN:
            return paren_expr();
        case TokenType::IF:
            return if_expr();
        default:
            throw throwable_error("Unknown token found",  "expression", peek().get_lexeme(),
                                                                        peek().get_line());
    }
}

// identifier => identifier
//            => identifier '(' expression* ')'
std::unique_ptr<ExprAST>
Parser::identifier()
{
    auto ident_token = advance();

    // variable reference
    if (peek().get_type() != TokenType::LEFT_PAREN)
        return std::make_unique<VariableExprAST>(ident_token.get_identifier());
    
    // function call
    advance(); // consume '('
    std::vector<std::unique_ptr<ExprAST>> args;
    while (peek().get_type() != TokenType::RIGHT_PAREN) {
        if (auto arg = expression())
            args.push_back(std::move(arg));
        else
            return nullptr;
    }

    advance(); // consume ')'
    return std::make_unique<CallExprAST>(ident_token.get_identifier(), std::move(args));
}

// number_expr => number
std::unique_ptr<ExprAST>
Parser::number_expr()
{
    auto num_token = peek();
    auto result = std::make_unique<NumberExprAST>(num_token.get_num());
    advance();
    return result;
}

// paren_expr => '(' expression ')'
std::unique_ptr<ExprAST>
Parser::paren_expr()
{
    advance();
    auto contents = expression();
    if (!contents)
        return nullptr; // TODO: Throw an exception: "Expression expected or not found"
    
    if (peek().get_type() != TokenType::RIGHT_PAREN)
        throw throwable_error("Syntax error", ")", peek().get_lexeme(), peek().get_line());

    advance();
    return contents;
}

// if_expr => 'if' expression 'then' expression 'else' expression
std::unique_ptr<ExprAST>
Parser::if_expr()
{
    advance(); // consume 'if'
    auto conditional = expression();
    if (!conditional)
        return nullptr;
    
    if (peek().get_type() != TokenType::THEN)
        throw throwable_error("Syntax error", "then", peek().get_lexeme(), peek().get_line());
    
    advance(); // consume 'then'
    auto then = expression();
    if (!then)
        return nullptr;

    if (peek().get_type() != TokenType::ELSE)
        throw throwable_error("Syntax error", "else", peek().get_lexeme(), peek().get_line());
    
    advance(); // consume 'else'
    auto else_ = expression();
    if (!else_)
        return nullptr;
    
    return std::make_unique<IfExprAST>(std::move(conditional), std::move(then), std::move(else_));
}

Token
Parser::peek()
{
    return this->tokens.at(this->curr_token);
}

Token
Parser::advance()
{
    return this->tokens.at(this->curr_token++);
}

bool
Parser::at_end()
{
    return peek().get_type() == TokenType::_EOF;
}

// Returns the precedence of the pending binary token
int
Parser::token_precedence(Token token)
{
    int prec = this->binary_op_precedences[token.get_type()];
    if (prec <= 0) return -1;
    return prec;
}

// returns true if the token is a binary op in the Parser's table
bool
Parser::is_binary_op(Token token)
{
    return this->binary_op_precedences[token.get_type()] > 0;
}

SmolParserError
Parser::throwable_error(const std::string &msg, const std::string &expected,
        const std::string &found, int line)
{
    return SmolParserError(msg, expected, found, line);
}