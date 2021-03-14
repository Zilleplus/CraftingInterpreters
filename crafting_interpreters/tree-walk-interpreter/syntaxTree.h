#pragma once
#include <memory> 
#include <vector>
#include <sstream>
#include <string>
#include <variant>

#include "tokens.h"
#include "variantOverload.h"

namespace lox {

class Expression;
class ExpressionVisitor;
class Literal;
class BinaryExpr;
class UnaryExpr;
class Grouping;
class Variable;
class Assignment;

class Statement;
class StatementVisitor;
class ExpressionStatement;
class PrintStatement;
class Block;

class Declaration;
class DeclarationVisitor;
class VariableDeclaration;

class Expression {
   public:
    virtual void Accept(ExpressionVisitor& vis) = 0;
    virtual ~Expression() = default;
};

class Statement {
   public:
    virtual void Accept(StatementVisitor& s) = 0;
    virtual ~Statement() = default;
};

class ExpressionVisitor {
   public:
    void Visit(Expression& expr) { expr.Accept(*this); }
    virtual ~ExpressionVisitor() = default;
    virtual void Visit(Literal&) = 0;
    virtual void Visit(BinaryExpr&) = 0;
    virtual void Visit(UnaryExpr&) = 0;
    virtual void Visit(Grouping&) = 0;
    virtual void Visit(Variable&) = 0;
    virtual void Visit(Assignment&) = 0;
};

struct StatementVisitor {
    virtual void Visit(Statement& expr) { expr.Accept(*this); }
    virtual ~StatementVisitor() = default;
    virtual void Visit(PrintStatement&) = 0;
    virtual void Visit(ExpressionStatement&) = 0;
    virtual void Visit(VariableDeclaration& vdecl) = 0;
    virtual void Visit(Block& blk) = 0;
};

class BinaryExpr final : public Expression {
   public:
    std::unique_ptr<Expression> Left;
    std::unique_ptr<Expression> Right;
    Token Tok;
    BinaryExpr(std::unique_ptr<Expression>&& l, std::unique_ptr<Expression>&& r,
               Token tok);

    virtual void Accept(ExpressionVisitor& vis) override { vis.Visit(*this); }
};

class UnaryExpr final : public Expression {
   public:
    std::unique_ptr<Expression> Expr;
    Token Op;
    UnaryExpr(std::unique_ptr<Expression>&& e, Token op);

    virtual void Accept(ExpressionVisitor& vis) override { vis.Visit(*this); }
};

class Grouping final : public Expression {
   public:
    std::unique_ptr<Expression> Expr;
    Grouping(std::unique_ptr<Expression>&& e);

    virtual void Accept(ExpressionVisitor& vis) override { vis.Visit(*this); }
};

class Variable final : public Expression {
    public:
     Token Name;
     Variable(Token name);

    virtual void Accept(ExpressionVisitor& vis) override { vis.Visit(*this); }
};

class Literal final : public Expression {
   public:
    using ValueType = std::variant<std::string, double, bool, std::monostate>;
    ValueType Value;
    Literal(ValueType val) : Value(val) {}

    virtual void Accept(ExpressionVisitor& vis) override { vis.Visit(*this); }
};

class ExpressionStatement final : public Statement {
   public:
    std::unique_ptr<Expression> Expr;
    ExpressionStatement(std::unique_ptr<Expression>&& e);

    virtual void Accept(StatementVisitor& vis) override { vis.Visit(*this); }
};

class PrintStatement final : public Statement {
   public:
    std::unique_ptr<Expression> Expr;
    PrintStatement(std::unique_ptr<Expression>&& expr);

    virtual void Accept(StatementVisitor& vis) override { vis.Visit(*this); }
};

class VariableDeclaration final : public Statement {
    public:
    std::unique_ptr<Expression> Initializer;
    Token Name;
    VariableDeclaration(Token name, std::unique_ptr<Expression>&& initializer);

    virtual void Accept(StatementVisitor& vis) override { vis.Visit(*this); }
};

class Assignment final : public Expression{
    public:
    std::unique_ptr<Expression> Expr;
    Token Name;
    Assignment(Token name, std::unique_ptr<Expression>&& expr);

    virtual void Accept(ExpressionVisitor& vis) override { vis.Visit(*this); }
};

class Block final : public Statement{
    public:
    std::vector<std::unique_ptr<Statement>> Statements;
    Block(std::vector<std::unique_ptr<Statement>>&& statements)
        : Statements(std::move(statements))
    {
    }

    virtual void Accept(StatementVisitor& vis) override { vis.Visit(*this); }
};

void print(Expression& expr);

}  // namespace lox
