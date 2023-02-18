#include "syntaxTree.h"
#include <sstream>
#include <string>
#include <iostream>
#include <variant>

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

ExpressionStatement::ExpressionStatement(std::unique_ptr<Expression>&& e)
    : Expr(std::move(e))
{
}
    

class AstSerializer final : public ExpressionVisitor{
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
        ExpressionVisitor::Visit(*bin.Left);
        ss_ << " ";
        ExpressionVisitor::Visit(*bin.Right);
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
        ExpressionVisitor::Visit(*gr.Expr);
        ss_ << ")";
    }

    virtual void Visit(Variable& var) override
    {
        ss_ << "(";
        ss_ << std::get<1>(var.Name.Data); // take the string...
        ss_ << ")";
    }

    virtual void Visit(Assignment& var) override
    {
        ss_ << "(= ";
        ss_ << var.Name.Lexeme;
        ss_ << " ";
        ExpressionVisitor::Visit(*var.Expr);
        ss_ << ")";
    }

    virtual void Visit(Logical& lg) override
    {
        ss_ << "(";
        ss_ << lg.Op.Lexeme;
        ss_ << " ";
        ExpressionVisitor::Visit(*lg.Left);
        ss_ << " ";
        ExpressionVisitor::Visit(*lg.Right);
        ss_ << " )";
    }

    virtual void Visit(Call& call) override
    {
        ss_ << "(Call ";
        ExpressionVisitor::Visit(*call.Callee);
        for(auto& a : call.Arguments)
        {
            ExpressionVisitor::Visit(*a);
        }
        ss_ << ")";
    }

    std::string Serialize(Expression& expr)
    {
        ExpressionVisitor::Visit(expr);
        return ss_.str();
    }
};

void print(Expression& expr)
{
    AstSerializer printer;
    auto&& text = printer.Serialize(expr);
    std::cout << text << std::endl;
}

PrintStatement::PrintStatement(std::unique_ptr<Expression>&& expr)
    : Expr(std::move(expr))
{}

VariableDeclaration::VariableDeclaration(
        Token name,
        std::unique_ptr<Expression>&& initializer) : 
    Name(std::move(name)), Initializer(std::move(initializer))
{}

Variable::Variable(Token name)
    : Name(name)
{}

Assignment::Assignment(
        Token name,
        std::unique_ptr<Expression>&& expr) : 
    Name(std::move(name)), Expr(std::move(expr))
{}

}
