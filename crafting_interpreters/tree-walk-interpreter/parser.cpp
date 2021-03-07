#include "parser.h"
#include "syntaxTree.h"
#include "tokens.h"
#include <memory>
#include <utility>
#include "variantOverload.h"
#include <optional>
#include <functional>
#include <numeric>

namespace lox {

std::unique_ptr<Expression> Parser::Expr()
{
    return Equality();
}

bool Parser::Check(TokenType type) const
{
    if (IsAtEnd()) {
        return false;
    };
    return Peek().Type == type;
}

bool Parser::Match(std::initializer_list<TokenType> tokens)
{
    bool c = std::transform_reduce(
            tokens.begin(),
            tokens.end(),
            false,
            std::logical_or(),
            [&](auto x){return Parser::Check(x);});

    if (c) {
        Advance();
        return true;
    }

    return false;
}

Token Parser::Previous() const
{
    return tokens_[current_ - 1];
}

Token Parser::Peek() const
{
    return tokens_[current_];
}

bool Parser::IsAtEnd() const
{
    return current_ > std::size(tokens_) - 1;
}

Token Parser::Advance()
{
    if (!IsAtEnd()) {
        current_++;
    }
    return Previous();
}

std::unique_ptr<Expression> Parser::Equality()
{
    auto expr = Comparison();

    while (Match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        auto op = Previous();
        auto right = Comparison();

        expr = std::make_unique<BinaryExpr>(
            std::move(expr),
            std::move(right),
            op);
    }

    return expr;
}

std::unique_ptr<Expression> Parser::Comparison()
{
    auto expr = Term();

    while(Match({
                TokenType::GREATER,
                TokenType::GREATER_EQUAL,
                TokenType::LESS,
                TokenType::LESS_EQUAL}))
    {
        Token op = Previous();
        auto right = Term();
        expr = std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op);
    }

    return expr;
}

std::unique_ptr<Expression> Parser::Term()
{
    auto expr = Factor();

    while(Match({
                TokenType::MINUS,
                TokenType::PLUS})){
        Token op = Previous();
        expr = std::make_unique<BinaryExpr>(std::move(expr), Factor(), op);
    }

    return expr;
}

std::unique_ptr<Expression> Parser::Factor()
{
    auto expr = Unary();

    while(Match({
                TokenType::SLASH,
                TokenType::STAR})){
        Token op = Previous();
        expr = std::make_unique<BinaryExpr>(std::move(expr), Factor(), op);
    }

    return expr;
}

std::unique_ptr<Expression> Parser::Unary()
{
    if(Match({
                TokenType::BANG,
                TokenType::MINUS})){
        return std::make_unique<UnaryExpr>(Unary(), Previous());
    }

    return Primary();
}

std::unique_ptr<Expression> Parser::Primary()
{
    if(Match({TokenType::FALSE})) {return std::make_unique<Literal>(false);}
    if(Match({TokenType::TRUE})) {return std::make_unique<Literal>(true);}
    if(Match({TokenType::NIL})) {return std::make_unique<Literal>(std::monostate());}

    if(Match({TokenType::NUMBER, TokenType::STRING})){
        auto token_data = Previous().Data;
        auto lit_data = std::visit(overload{
            [](std::string lit){ return std::optional<Literal::ValueType>{lit};},
            [](double d) {return std::optional<Literal::ValueType>{d};},
            [](auto _) {return std::optional<Literal::ValueType>{};}
            },
            token_data);

        if(lit_data.has_value())
        {
            return std::make_unique<Literal>(*lit_data);
        }
    }

    if(Match({TokenType::LEFT_PAREN}))
    {
        auto expr = Expr();
        Consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return std::make_unique<Grouping>(std::move(expr));
    }
    
    throw Error(Peek(),"Expected Expression");
}

void Parser::Synchronize(){
    Advance(); // skip fault token

    while(!IsAtEnd()){
        if(Previous().Type==TokenType::SEMICOLON){
            return;
        }
        switch(Peek().Type)
        {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
            default:
                break;
        }

        Advance();
    }
}

}
