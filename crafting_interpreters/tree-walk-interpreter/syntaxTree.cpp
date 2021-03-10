#include "syntaxTree.h"
#include <sstream>
#include <string>
#include <iostream>

namespace lox {

BinaryExpr::BinaryExpr(
    std::unique_ptr<Expression>&& l,
    std::unique_ptr<Expression>&& r,
    Token tok)
    : Left(std::move(l))
    , Right(std::move(r))
    , Tok(tok)
{
}

UnaryExpr::UnaryExpr(
    std::unique_ptr<Expression>&& e,
    Token op)
    : Expr(std::move(e))
    , Op(op)
{
}

Grouping::Grouping(std::unique_ptr<Expression>&& e)
    : Expr(std::move(e))
{
}

class AstSerializer final : public Visitor{
private:
    std::stringstream ss_;

public:
    virtual void Visit(Literal& lit) override
    {
        auto text = std::visit(
            overload {
                [](const double b) { return std::to_string(b); },
                [](const std::string& s) { return s; },
                [](const std::monostate _){return std::string("nill");}},
            lit.Value);

        this->ss_ << "( literal " << text << " )";
    }

    virtual void Visit(BinaryExpr& bin) override
    {
        ss_ << "(";
        ss_ << ToString(bin.Tok.Type);
        ss_ << " ";
        Visitor::Visit(*bin.Left);
        ss_ << " ";
        Visitor::Visit(*bin.Right);
        ss_ << ")";
    }

    virtual void Visit(UnaryExpr& un) override
    {
        ss_ << "(";
        ss_ << ToString(un.Op.Type);
        ss_ << " ";
        un.Expr->Accept(*this);
        ss_ << ")";
    }

    virtual void Visit(Grouping& gr) override
    {
        ss_ << "(";
        Visitor::Visit(*gr.Expr);
        ss_ << ")";
    }

    std::string Serialize(Expression& expr)
    {
        Visitor::Visit(expr);
        return ss_.str();
    }
};

void print(Expression& expr)
{
    AstSerializer printer;
    auto&& text = printer.Serialize(expr);
    std::cout << text << std::endl;
}

}
