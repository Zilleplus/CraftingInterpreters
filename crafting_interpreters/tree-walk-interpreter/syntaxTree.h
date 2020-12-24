#pragma one

#include "tokens.h"
#include "variantOverload.h"
#include <variant>
#include <memory>

namespace lox {

class Literal;
class BinaryExpr;
class UnaryExpr;
class Grouping;

struct Visitor{
    virtual void Visit(Literal*) =0;
    virtual void Visit(BinaryExpr*) =0;
    virtual void Visit(UnaryExpr*) =0;
    virtual void Visit(Grouping*) =0;
};

// Should we use a recursive std::variant?
class Expression{
    public:
    virtual void Accept(Visitor vis) = 0;
    virtual ~Expression(){}
};

class BinaryExpr : Expression {
public:
    std::unique_ptr<Expression> Left;
    std::unique_ptr<Expression> Right;
    TokenType Op;
    BinaryExpr(
        std::unique_ptr<Expression>&& l,
        std::unique_ptr<Expression>&& r,
        TokenType op);

    virtual void Accept(Visitor vis) override{
        vis.Visit(this);
    }
};

class UnaryExpr : Expression {
public:
    std::unique_ptr<Expression> Expr;
    TokenType Op;
    UnaryExpr(
        std::unique_ptr<Expression>&& e,
        TokenType op);

    virtual void Accept(Visitor vis) override{
        vis.Visit(this);
    }
};

class Grouping : Expression {
public:
    const std::unique_ptr<Expression> Expr;
    Grouping(std::unique_ptr<Expression>&& e);

    virtual void Accept(Visitor vis) override{
        vis.Visit(this);
    }
};

class Literal : Expression{
public:
    using ValueType = std::variant<std::string, double>;
    const ValueType Value;
    Literal(ValueType val)
        : Value(val)
    {
    }

    virtual void Accept(Visitor vis) override{
        vis.Visit(this);
    }
};

// Not sure if this works, looks interesting though.
template<typename... TVis>
struct VisGen : public TVis...{
    VisGen(TVis... visitors){
    }
};


}
