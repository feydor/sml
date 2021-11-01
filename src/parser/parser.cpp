#include "parser.h"

void
Parser::parse_syntax()
{
    while (!at_end())
        ast.push_back(std::move(primary()));
}

std::unique_ptr<ExprAST>
Parser::primary()
{
    switch (peek().get_type()) {
        case TokenType::IDENTIFIER:
            return identifier();
        case TokenType::NUMBER:
            return number_expr();
        case TokenType::STRING:
            return string_expr();
        case TokenType::LEFT_PAREN:
            return paren_expr();
    }
}

std::unique_ptr<ExprAST>
Parser::number_expr()
{
    auto num_token = peek();
    auto result = std::make_unique<NumberExprAST>(num_token.get_num());
    advance();
    return std::move(result);
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