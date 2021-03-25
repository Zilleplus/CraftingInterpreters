#pragma once
#include <functional>
#include <iostream>
#include <string>
#include <variant>

#include "environment.h"
#include "foldVisitor.h"
#include "loxFunction.h"
#include "runtimeerror.h"
#include "syntaxTree.h"
#include "variantOverload.h"

namespace lox {

void ReportRunTimeError(RunTimeError);

class Interpreter : public StatementVisitor, public ExpressionVisitor {
   public:
    using TOut = std::variant<bool, double, std::string, LoxFunction>;
    std::stack<TOut> stack_;
    std::shared_ptr<Environment<TOut>> Globals = std::make_shared<Environment<TOut>>();
   private:
    std::shared_ptr<Environment<TOut>> environment_ = Globals; // By default use global scope.
    static TOut EvalUnExpr(Token t, TOut v);
    static TOut EvalLiteral(Literal& l);
    static TOut EvalBinExpr(Token t, TOut l, TOut r);
    static TOut EvalGroup(TOut v) { return v; }
    void EvalAnd(Logical& lg);
    void EvalOr(Logical& lg);

   public:
    virtual void Visit(Literal& l) override;
    virtual void Visit(BinaryExpr& b) override;
    virtual void Visit(UnaryExpr& u) override;
    virtual void Visit(Grouping& g) override;
    virtual void Visit(Variable& var) override;
    virtual void Visit(Assignment& ass) override;
    virtual void Visit(Logical& lg) override;
    virtual void Visit(Call& c) override;

    virtual void Visit(PrintStatement& p) override;
    virtual void Visit(ExpressionStatement& s) override;
    void ExecuteBlock(std::vector<std::shared_ptr<Statement>>&,
                      std::shared_ptr<Environment<TOut>>& env);
    virtual void Visit(Block& blk) override;
    virtual void Visit(IfStatement& ifm) override;
    virtual void Visit(While& whl) override;
    virtual void Visit(FunctionDeclaration& whl) override;
    virtual void Visit(VariableDeclaration& var) override;
    virtual void Visit(ReturnStatement&) override;

    TOut Eval(Expression& expr) {
        expr.Accept(*this);
        auto answer = stack_.top();
        stack_.pop();
        return answer;
    }

    void Execute(Statement& s) { s.Accept(*this); }
    void Interpret(std::vector<std::unique_ptr<Statement>>& statements);
};
}  // namespace lox
