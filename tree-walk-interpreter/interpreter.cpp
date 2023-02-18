#include "interpreter.h"

#include <string>

#include "loxFunction.h"
#include "return.h"

namespace lox {

using namespace std::string_literals;

template <typename T>
bool IsTruth(const T& val) {
    return std::visit(
        overload{[](bool b) { return b; }, [](auto _) { return false; }}, val);
}

void Interpreter::Visit(Literal& l) { stack_.push(EvalLiteral(l)); }

void Interpreter::Visit(BinaryExpr& b) {
    b.Left->Accept(*this);
    auto l = stack_.top();
    stack_.pop();

    b.Right->Accept(*this);
    auto r = stack_.top();
    stack_.pop();

    stack_.push(EvalBinExpr(b.Tok, l, r));
}

void Interpreter::Visit(UnaryExpr& u) {
    u.Expr->Accept(*this);
    auto e = stack_.top();
    stack_.pop();

    stack_.push(EvalUnExpr(u.Op, e));
}

void Interpreter::Visit(Grouping& g) {
    g.Expr->Accept(*this);
    auto e = stack_.top();
    stack_.pop();

    stack_.push(EvalGroup(e));
}

using TOut = Interpreter::TOut;
static void RError(Token t, std::string message) {
    throw RunTimeError{t, std::string("Unsupported operation between doubles")};
}

TOut Interpreter::EvalLiteral(Literal& l) {
    return std::visit(overload{[](std::monostate _) { return TOut("Nil"); },
                               [](auto v) { return TOut(v); }},
                      l.Value);
}

TOut Interpreter::EvalUnExpr(Token t, TOut v) {
    return std::visit(
        overload{
            [&t](double d) -> TOut {
                switch (t.Type) {
                    case TokenType::MINUS:
                        return {-d};
                    default:
                        break;
                }
                RError(t, "Operation not supported for doubles.");
                return {"Nil"};
            },
            [&t](bool b) -> TOut {
                switch (t.Type) {
                    case TokenType::BANG:
                        return !b;
                    default:
                        break;
                }
                RError(t, "Operation not supported for bools.");
                return {"Nil"};
            },
            [&t](auto _) -> TOut {
                if (t.Type == TokenType::BANG) {
                    // TODO:: If Nill we should return false.
                }
                RError(t, "This Unitary operator needs either bool or double.");
                return {"Nil"};
            }},
        v);
}

static TOut EvalBinDoubleExpr(Token& t, double a, double b) {
    switch (t.Type) {
        case TokenType::PLUS:
            return {a + b};
        case TokenType::MINUS:
            return {a - b};
        case TokenType::STAR:
            return {a * b};
        case TokenType::SLASH:
            return {a / b};
        case TokenType::GREATER:
            return a > b;
        case TokenType::GREATER_EQUAL:
            return a >= b;
        case TokenType::LESS:
            return a < b;
        case TokenType::LESS_EQUAL:
            return a <= b;
        case TokenType::BANG_EQUAL:
            return a != b;
        case TokenType::EQUAL_EQUAL:
            return a == b;
        default:
            return TOut("Nil");
    }
}
static TOut EvalBinBoolExpr(Token& t, const bool a, const bool b) {
    switch (t.Type) {
        case TokenType::AND:
            return {a && b};
        case TokenType::OR:
            return {a || b};
        case TokenType::BANG_EQUAL:
            return a != b;
        case TokenType::EQUAL_EQUAL:
            return a == b;
        default:
            break;
    }
    RError(t, "Operation not supported for bools.");
    return {"Nil"};
}

static TOut EvalBinStringExpr(Token& t, const std::string& s_l,
                              const std::string& s_r) {
    switch (t.Type) {
        case TokenType::BANG_EQUAL:
            return s_l != s_r;
        case TokenType::EQUAL_EQUAL:
            return s_l == s_r;
        case TokenType::PLUS:
            return s_l + s_r;
        default:
            break;
    }
    RError(t, "Operator not supported for strings.");
    return {"Nil"};
}

TOut Interpreter::EvalBinExpr(Token t, TOut l, TOut r) {
    return std::visit(
        overload{
            [&r, &t](double a) -> TOut {
                return std::visit(
                    overload{
                        [&t, a](double b) -> TOut {
                            return EvalBinDoubleExpr(t, a, b);
                        },
                        [](auto _) -> TOut { return TOut("Nil"); },
                    },
                    r);
            },
            [&r, &t](bool a) -> TOut {
                return std::visit(
                    overload{
                        [&t, a](bool b) -> TOut {
                            return EvalBinBoolExpr(t, a, b);
                        },
                        [&t](auto _) -> TOut {
                            RError(t,
                                   "Left and Right are not of the same type.");
                            return {"Nil"};
                        },
                    },
                    r);
            },
            [&r, &t](std::string s_l) -> TOut {
                return std::visit(
                    overload{
                        [s_l, &t](std::string s_r) -> TOut {
                            return EvalBinStringExpr(t, s_l, s_r);
                        },
                        [&t](auto _) -> TOut {
                            RError(t,
                                   "Left and Right are not of the same type.");
                            return {"Nil"};
                        }},
                    r);
            },
            [&t](LoxFunction& l) -> TOut {
                RError(t, "Unexpected callable");
                return {"Nil"};
            }},
        l);
}

void Interpreter::Visit(Variable& var) {
    stack_.push(LookUpVariable(var.Name, &var));
}

void Interpreter::Visit(VariableDeclaration& var) {
    TOut value;
    if (var.Initializer != nullptr) {
        value = Eval(*(var.Initializer));
    }
    environment_->Define(var.Name.Lexeme, value);
}

void Interpreter::Visit(PrintStatement& p) {
    auto val = Eval(*p.Expr);
    std::visit(overload{[](LoxFunction& c) {},
                        [](auto v) { std::cout << v << std::endl; }},
               val);
}

void Interpreter::Visit(ExpressionStatement& s) { auto val = Eval(*s.Expr); }

void Interpreter::Interpret(
    std::vector<std::unique_ptr<Statement>>& statements) {
    try {
        for (auto& s : statements) {
            Execute(*s);
        }

    } catch (RunTimeError rte) {
        while (!std::empty(stack_)) {
            stack_.pop();
        }
        ReportRunTimeError(rte);
    }
}

void Interpreter::Visit(Assignment& ass) {
    auto val = Eval(*ass.Expr);
    auto distance = locals.find(&ass);
    if (distance != locals.end()) {
        environment_->AssignAt(distance->second, ass.Name, val);
    } else {
        Globals->Assign(ass.Name, val);
    }
    stack_.push(val);
}

void Interpreter::ExecuteBlock(
    std::vector<std::shared_ptr<Statement>>& statements,
    std::shared_ptr<Environment<TOut>>& env) {
    auto old_env = environment_;
    environment_ = env;  // Assign the pointer.
    try {
        for (auto& s : statements) {
            Execute(*s);
        }
    } catch (...) {
        environment_ = old_env;  // reset the pointer on failure.
        throw;
    }
    environment_ = old_env;
}

void Interpreter::Visit(Block& blk) {
    auto env = std::make_shared<Environment<TOut>>(environment_);
    ExecuteBlock(blk.Statements, env);
}

void Interpreter::Visit(IfStatement& ifm) {
    if (IsTruth(Eval(*ifm.Condition))) {
        Execute(*ifm.ThenBranch);
    } else if (ifm.ElseBranch != nullptr) {
        Execute(*ifm.ElseBranch);
    }
}

void Interpreter::EvalOr(Logical& lg) {
    auto left = Eval(*lg.Left);
    if (IsTruth(left)) {
        stack_.push(left);
        return;
    }
    stack_.push(Eval(*lg.Right));
    return;
}

void Interpreter::EvalAnd(Logical& lg) {
    auto left = Eval(*lg.Left);
    if (IsTruth(left)) {
        stack_.push(Eval(*lg.Right));
        return;
    }
    stack_.push(left);  // return false;
}

void Interpreter::Visit(Logical& lg) {
    if (lg.Op.Type == TokenType::OR) {
        EvalOr(lg);
    }
    if (lg.Op.Type == TokenType::AND) {
        EvalAnd(lg);
    }
}

void Interpreter::Visit(While& whl) {
    while (IsTruth(Eval(*whl.Condition))) {
        Execute(*whl.Body);
    }
}

void Interpreter::Visit(Call& c) {
    auto callee = Eval(*c.Callee);

    std::vector<TOut> arguments;
    for (auto& arg : c.Arguments) {
        arguments.push_back(Eval(*arg));
    }

    stack_.push(std::visit(
        overload{[&](LoxFunction& lf) -> TOut {
                     if (std::size(arguments) != lf.Arity()) {
                         std::string error_message =
                             "Expected"s + std::to_string(lf.Arity()) +
                             " arguments but got "s +
                             std::to_string(std::size(arguments)) + "."s;

                         throw RunTimeError{c.Paren, std::move(error_message)};
                     }
                     return lf.Call(*this, arguments);
                 },
                 [&c](auto _) -> TOut {
                     throw RunTimeError{c.Paren,
                                        "Can only call functions and classes"};
                     return TOut{"Nil"};
                 }},
        callee));
}

void Interpreter::Visit(FunctionDeclaration& fd) {
    environment_->Define(fd.Name.Lexeme, {LoxFunction(fd, environment_)});
}

void Interpreter::Visit(ReturnStatement& rstm) {
    TOut value;
    if (rstm.Value != nullptr) {
        value = Eval(*rstm.Value);
    }

    throw Return(value);
}

void Interpreter::Resolve(Expression* expr, int depth) {
    locals.insert({expr, depth});
}

TOut Interpreter::LookUpVariable(Token name, Variable* expr) {
    auto distance = locals.find(expr);
    if (distance != locals.end()) {
        return environment_->GetAt(distance->second, name);
    }
    return Globals->Get(name);
}

}  // namespace lox
