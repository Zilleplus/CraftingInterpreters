#pragma once

#include "interpreter.h"
#include "syntaxTree.h"
#include <map>
#include <vector>

namespace lox {

class Resolver : ExpressionVisitor, StatementVisitor {
    Interpreter& interpreter_;

    std::vector<std::map<std::string, bool>> scopes;

    public:
    Resolver(Interpreter& interpreter) : interpreter_(interpreter) {}

    void BeginScope();
    void EndScope();
    void Declare(Token name);
    void Define(Token name);
    void Resolve(Expression&);
    void Resolve(Statement&);
    void Resolve(std::vector<std::unique_ptr<Statement>>&);
    void Resolve(std::vector<std::shared_ptr<Statement>>&);

    void ResolveLocal(Expression* expr, Token Name);
    void ResolveFunction(FunctionDeclaration&);

    virtual void Visit(Literal&) override;
    virtual void Visit(BinaryExpr&) override;
    virtual void Visit(UnaryExpr&) override;
    virtual void Visit(Grouping&) override;
    virtual void Visit(Variable&) override;
    virtual void Visit(Assignment&) override;
    virtual void Visit(Logical&) override;
    virtual void Visit(Call&) override;
    virtual void Visit(PrintStatement&) override;
    virtual void Visit(ExpressionStatement&) override;
    virtual void Visit(VariableDeclaration& vdecl) override;
    virtual void Visit(Block& blk) override;
    virtual void Visit(IfStatement&) override;
    virtual void Visit(While&) override;
    virtual void Visit(FunctionDeclaration&) override;
    virtual void Visit(ReturnStatement&) override;
};

}  // namespace lox
