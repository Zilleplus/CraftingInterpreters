#include <functional>
#include <stack>
#include <variant>

#include "syntaxTree.h"

namespace lox {

template <typename T>
class FoldVisitor : public Visitor {
    std::stack<T> stack_;
    std::function<T(Literal&)> literal_;
    std::function<T(Token, T, T)> bin_expr_;  // op , left, right
    std::function<T(Token, T)> un_expr_;
    std::function<T(T)> gr_;

    void ClearStack() {
        while (!std::empty(stack_)) {
            stack_.pop();
        }
    }

   public:
    FoldVisitor(std::function<T(Literal&)> literal,
                std::function<T(Token, T, T)> bin_expr,  // op , left, right
                std::function<T(Token, T)> un_expr, std::function<T(T)> gr)
        : literal_(literal), bin_expr_(bin_expr), un_expr_(un_expr), gr_(gr) {}

    virtual ~FoldVisitor() override {}

    virtual void Visit(Literal& l) override {
        try {
            stack_.push(literal_(l));
        } catch (...) {
            ClearStack();
            throw;
        }
    }

    virtual void Visit(BinaryExpr& b) override {
        try {
            b.Left->Accept(*this);
            auto l = stack_.top();
            stack_.pop();

            b.Right->Accept(*this);
            auto r = stack_.top();
            stack_.pop();

            stack_.push(bin_expr_(b.Tok, l, r));
        } catch (...) {
            ClearStack();
            throw;
        }
    }

    virtual void Visit(UnaryExpr& u) override {
        try {
            u.Accept(*this);
            auto e = stack_.top();
            stack_.pop();

            stack_.push(un_expr_(u.Op, e));
        } catch (...) {
            ClearStack();
            throw;
        }
    }

    virtual void Visit(Grouping& g) override {
        try {
            g.Accept(*this);
            auto e = stack_.top();
            stack_.pop();

            stack_.push(gr_(e));
        } catch (...) {
            ClearStack();
            throw;
        }
    }

    T Visit(Expression& e) {
        try {
            e.Accept(*this);
            T out = stack_.top();
            stack_.pop();
            return out;
        } catch (...) {
            ClearStack();
            throw;
        }
    }
};

}  // namespace lox
