#include "parser.h"

#include <functional>
#include <memory>
#include <numeric>
#include <optional>
#include <utility>

#include "syntaxTree.h"
#include "tokens.h"
#include "variantOverload.h"

namespace lox {

std::unique_ptr<Expression> Parser::Expr() { return Assign(); }

bool Parser::Check(TokenType type) const {
    if (IsAtEnd()) {
        return false;
    };
    return Peek().Type == type;
}

bool Parser::Match(std::initializer_list<TokenType> tokens) {
    bool c = std::transform_reduce(tokens.begin(), tokens.end(), false,
                                   std::logical_or(),
                                   [&](auto x) { return Parser::Check(x); });

    if (c) {
        Advance();
        return true;
    }

    return false;
}

Token Parser::Previous() const { return tokens_[current_ - 1]; }

Token Parser::Peek() const { return tokens_[current_]; }

std::optional<Token> Parser::PeekNext() const {
    auto next_current = current_ + 1;
    return (next_current < std::size(tokens_))
               ? std::optional<Token>(tokens_[next_current])
               : std::optional<Token>();
}

bool Parser::IsAtEnd() const { return current_ >= std::size(tokens_) - 1; }

Token Parser::Advance() {
    if (!IsAtEnd()) {
        current_++;
    }
    return Previous();
}

std::unique_ptr<Expression> Parser::Equality() {
    auto expr = Comparison();

    while (Match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        auto op = Previous();
        auto right = Comparison();

        expr =
            std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op);
    }

    return expr;
}

std::unique_ptr<Expression> Parser::Comparison() {
    auto expr = Term();

    while (Match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                  TokenType::LESS_EQUAL})) {
        Token op = Previous();
        auto right = Term();
        expr =
            std::make_unique<BinaryExpr>(std::move(expr), std::move(right), op);
    }

    return expr;
}

std::unique_ptr<Expression> Parser::Term() {
    auto expr = Factor();

    while (Match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = Previous();
        expr = std::make_unique<BinaryExpr>(std::move(expr), Factor(), op);
    }

    return expr;
}

std::unique_ptr<Expression> Parser::Factor() {
    auto expr = Unary();

    while (Match({TokenType::SLASH, TokenType::STAR})) {
        Token op = Previous();
        expr = std::make_unique<BinaryExpr>(std::move(expr), Factor(), op);
    }

    return expr;
}

std::unique_ptr<Expression> Parser::Unary() {
    if (Match({TokenType::BANG, TokenType::MINUS})) {
        return std::make_unique<UnaryExpr>(Unary(), Previous());
    }

    return Primary();
}

std::unique_ptr<Expression> Parser::Primary() {
    if (Match({TokenType::FALSE})) {
        return std::make_unique<Literal>(false);
    }
    if (Match({TokenType::TRUE})) {
        return std::make_unique<Literal>(true);
    }
    if (Match({TokenType::NIL})) {
        return std::make_unique<Literal>(std::monostate());
    }

    if (Match({TokenType::NUMBER, TokenType::STRING})) {
        auto token_data = Previous().Data;
        auto lit_data = std::visit(
            overload{
                [](std::string lit) {
                    return std::optional<Literal::ValueType>{lit};
                },
                [](double d) { return std::optional<Literal::ValueType>{d}; },
                [](auto _) { return std::optional<Literal::ValueType>{}; }},
            token_data);

        if (lit_data.has_value()) {
            return std::make_unique<Literal>(*lit_data);
        }
    }

    if (Match({TokenType::IDENTIFIER})) {
        return std::make_unique<Variable>(Previous());
    }

    if (Match({TokenType::LEFT_PAREN})) {
        auto expr = Expr();
        Consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw Error(Peek(), "Expected Expression");
}

void Parser::Synchronize() {
    Advance();  // skip fault token

    while (!IsAtEnd()) {
        if (Previous().Type == TokenType::SEMICOLON) {
            return;
        }
        switch (Peek().Type) {
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

std::unique_ptr<Statement> Parser::ExprSmt() {
    auto expr = std::make_unique<ExpressionStatement>(Expr());
    Consume(TokenType::SEMICOLON, "Expected ';' after value.");
    return std::move(expr);
}

std::unique_ptr<Statement> Parser::PrintSmt() {
    auto expr = std::make_unique<PrintStatement>(Expr());
    Consume(TokenType::SEMICOLON, "Expected ';' after expression.");
    return std::move(expr);
}

std::unique_ptr<Statement> Parser::Smt() {
    if (Match({TokenType::PRINT})) {
        return PrintSmt();
    }
    if (Match({TokenType::FOR})) {
        return Fr();
    }
    if (Match({TokenType::WHILE})) {
        return Whl();
    }
    if (Match({TokenType::LEFT_BRACE})) {
        return Blck();
    }
    if (Match({TokenType::IF})) {
        return IfSmt();
    }

    return ExprSmt();
}

std::unique_ptr<Statement> Parser::VarDeclaration() {
    auto name = Consume(TokenType::IDENTIFIER, "Expected variable name.");

    std::unique_ptr<Expression> initializer = nullptr;
    if (Match({TokenType::EQUAL})) {
        initializer = Expr();
    }

    Consume(TokenType::SEMICOLON, "Expect ';' after variable declaration");

    return std::make_unique<VariableDeclaration>(name, std::move(initializer));
}

std::unique_ptr<Statement> Parser::Decl() {
    try {
        if (Match({TokenType::VAR})) {
            return VarDeclaration();
        }

        return Smt();
    } catch (ParseError error) {
        Synchronize();
        return nullptr;
    }
}

std::unique_ptr<Expression> Parser::Assign() {
    std::unique_ptr<Expression> expr = Or();
    if (Match({TokenType::EQUAL})) {
        Token equals = Previous();
        std::unique_ptr<Expression> value = Assign();

        Variable* var = dynamic_cast<Variable*>(expr.get());
        if (var != nullptr) {
            return std::make_unique<Assignment>(var->Name, std::move(value));
        }

        Error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::unique_ptr<Statement> Parser::Blck() {
    std::vector<std::unique_ptr<Statement>> statements;

    while (!Check(TokenType::RIGHT_BRACE) && !IsAtEnd()) {
        statements.push_back(Decl());
    }

    Consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return std::make_unique<Block>(std::move(statements));
}

std::unique_ptr<Statement> Parser::IfSmt() {
    Consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    std::unique_ptr<Expression> condition = Expr();
    Consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

    std::unique_ptr<Statement> thenBranch = Smt();
    std::unique_ptr<Statement> elseBranch = nullptr;
    if (Match({TokenType::ELSE})) {
        elseBranch = Smt();
    }

    return std::make_unique<IfStatement>(
        std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<Expression> Parser::And() {
    std::unique_ptr<Expression> expr = Equality();

    while (Match({TokenType::AND})) {
        Token op = Previous();
        std::unique_ptr<Expression> right = Equality();
        expr = std::make_unique<Logical>(op, std::move(expr), std::move(right));
    }

    return expr;
}

std::unique_ptr<Expression> Parser::Or() {
    std::unique_ptr<Expression> expr = And();

    while (Match({TokenType::OR})) {
        Token op = Previous();
        std::unique_ptr<Expression> right = Equality();
        expr = std::make_unique<Logical>(op, std::move(expr), std::move(right));
    }

    return expr;
}

std::unique_ptr<Statement> Parser::Whl() {
    Consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    std::unique_ptr<Expression> condition = Expr();
    Consume(TokenType::RIGHT_PAREN, "Expect ')' after 'while'.");
    std::unique_ptr<Statement> body = Smt();

    return std::make_unique<While>(std::move(condition), std::move(body));
}

std::unique_ptr<Statement> Parser::Fr() {
    Consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

    std::unique_ptr<Statement> initializer;
    if (Match({TokenType::SEMICOLON})) {
        initializer = nullptr;
    } else if (Match({TokenType::VAR})) {
        initializer = VarDeclaration();
    } else {
        initializer = ExprSmt();
    }

    std::unique_ptr<Expression> condition;
    if (!Check(TokenType::SEMICOLON)) {
        condition = Expr();
    }
    Consume(TokenType::SEMICOLON, "Expect ';' after loop condition");

    std::unique_ptr<Expression> increment;
    if (!Check(TokenType::SEMICOLON)) {
        increment = Expr();
    }
    Match({TokenType::SEMICOLON}); // remove the optional ;
    Consume(TokenType::RIGHT_PAREN, "Expect ')' after 'for'.");

    std::unique_ptr<Block> body = std::make_unique<Block>();
    if (increment != nullptr)

    {
        body->Statements.push_back(Smt());
        body->Statements.push_back(std::make_unique<ExpressionStatement>(std::move(increment)));
    }

    if (condition == nullptr) {
        condition = std::make_unique<Literal>(true);
    }

    std::unique_ptr<Statement> loop =
        std::make_unique<While>(std::move(condition), std::move(body));

    if(initializer!=nullptr)
    {
        std::unique_ptr<Block> loop_with_init =  std::make_unique<Block>();
        loop_with_init->Statements.push_back(std::move(initializer));
        loop_with_init->Statements.push_back(std::move(loop));
        return loop_with_init;
    }

    return loop;
}

}  // namespace lox
