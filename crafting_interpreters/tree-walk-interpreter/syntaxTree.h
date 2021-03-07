#pragma once
#include "tokens.h"
#include "variantOverload.h"
#include <memory>
#include <sstream>
#include <string>
#include <variant>

namespace lox {

class Literal;
class BinaryExpr;
class UnaryExpr;
class Grouping;
class Visitor;

// Not using the std::variant, as the std::visit
// behaves weird when inhr. from it. (recursive types...)
// Further investigation needed.
class Expression {
public:
    virtual void Accept(Visitor& vis) = 0;
    virtual ~Expression() { }
};

struct Visitor {
    inline void Visit(Expression& expr) {expr.Accept(*this);}
    virtual ~Visitor()= default;
    virtual void Visit(Literal&) = 0;
    virtual void Visit(BinaryExpr&) = 0;
    virtual void Visit(UnaryExpr&) = 0;
    virtual void Visit(Grouping&) = 0;
};

class BinaryExpr final : public Expression {
public:
    std::unique_ptr<Expression> Left;
    std::unique_ptr<Expression> Right;
    Token Tok;
    BinaryExpr(
        std::unique_ptr<Expression>&& l,
        std::unique_ptr<Expression>&& r,
        Token tok);

    virtual void Accept(Visitor& vis) override
    {
        vis.Visit(*this);
    }
};

class UnaryExpr final : public Expression {
public:
    std::unique_ptr<Expression> Expr;
    Token Op;
    UnaryExpr(
        std::unique_ptr<Expression>&& e,
        Token op);

    virtual void Accept(Visitor& vis) override
    {
        vis.Visit(*this);
    }
};

class Grouping final : public Expression {
public:
    std::unique_ptr<Expression> Expr;
    Grouping(std::unique_ptr<Expression>&& e);

    virtual void Accept(Visitor& vis) override
    {
        vis.Visit(*this);
    }
};

class Literal final : public Expression {
public:
    using ValueType = std::variant<std::string, double, bool, std::monostate>;
    ValueType Value;
    Literal(ValueType val)
        : Value(val)
    {
    }

    virtual void Accept(Visitor& vis) override
    {
        vis.Visit(*this);
    }
};

// Not sure if this works, looks interesting though.
template <typename... TVis>
struct VisGen : public TVis... {
    VisGen(TVis... visitors)
    {
    }
};

void print(Expression& expr);

}
