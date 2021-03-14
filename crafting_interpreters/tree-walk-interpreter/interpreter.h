#pragma once
#include <functional>
#include <iostream>
#include <string>
#include <variant>

#include "environment.h"
#include "foldVisitor.h"
#include "syntaxTree.h"
#include "variantOverload.h"
#include "runtimeerror.h"

namespace lox {

void ReportRunTimeError(RunTimeError);

class Interpreter : public StatementVisitor,
                    public ExpressionVisitor
                    {
   public:
    using TOut = std::variant<bool, double, std::string>;
    std::stack<TOut> stack_;

   private:
    std::unique_ptr<Environment<TOut>> environment_;
    static TOut EvalUnExpr(Token t, TOut v);
    static TOut EvalLiteral(Literal& l);
    static TOut EvalBinExpr(Token t, TOut l, TOut r);
    static TOut EvalGroup(TOut v) { return v; }

   public:
    virtual void Visit(Literal& l) override;
    virtual void Visit(BinaryExpr& b) override;
    virtual void Visit(UnaryExpr& u) override;
    virtual void Visit(Grouping& g) override;
    virtual void Visit(Variable& var) override;
    virtual void Visit(Assignment& ass) override;

    virtual void Visit(PrintStatement& p) override;
    virtual void Visit(ExpressionStatement& s) override;
    virtual void Visit(Block& blk) override;

    virtual void Visit(VariableDeclaration& var) override;
    Interpreter();

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
