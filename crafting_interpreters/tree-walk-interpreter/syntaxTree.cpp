#include "syntaxTree.h"

namespace lox {

BinaryExpr::BinaryExpr(
    std::unique_ptr<Expression>&& l,
    std::unique_ptr<Expression>&& r,
    TokenType op)
    : Left(std::move(l))
    , Right(std::move(r))
    , Op(op)
{
}

UnaryExpr::UnaryExpr(
    std::unique_ptr<Expression>&& e,
    TokenType op)
    : Expr(std::move(e))
    , Op(op)
{}

Grouping::Grouping(std::unique_ptr<Expression>&& e)
        : Expr(std::move(e))
{}

}
