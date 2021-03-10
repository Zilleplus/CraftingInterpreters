#pragma once
#include <functional>
#include <iostream>
#include <string>
#include <variant>

#include "foldVisitor.h"
#include "syntaxTree.h"
#include "variantOverload.h"

namespace lox {

struct RunTimeError {
    Token Operator;
    std::string ErrorMsg;
};

void ReportRunTimeError(RunTimeError);

class Interpreter : public Visitor {
   public:
    using TOut = std::variant<bool, double, std::string>;
    std::stack<TOut> stack_;

   private:
    FoldVisitor<TOut> vis;

    static TOut EvalUnExpr(Token t, TOut v);

    static TOut EvalLiteral(Literal& l);

    static TOut EvalBinExpr(Token t, TOut l, TOut r);

    static TOut EvalGroup(TOut v) { return v; }

   public:
    virtual void Visit(Literal& l) override;
    virtual void Visit(BinaryExpr& b) override;
    virtual void Visit(UnaryExpr& u) override;
    virtual void Visit(Grouping& g) override;

    Interpreter() : vis(EvalLiteral, EvalBinExpr, EvalUnExpr, EvalGroup) {}

    TOut Eval(Expression& expr) { return vis.Visit(expr); }

    void Interpret(Expression& expr) {
        try {
            auto e = Eval(expr);
            std::visit(overload{[](auto v) { std::cout << v << std::endl; }},
                       e);

        } catch (RunTimeError rte) {
            while (!std::empty(stack_)) {
                stack_.pop();
            }
            ReportRunTimeError(rte);
        }
    }
};
}  // namespace lox
