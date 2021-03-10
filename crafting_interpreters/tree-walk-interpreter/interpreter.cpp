#include "interpreter.h"

namespace lox {

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
        u.Accept(*this);
        auto e = stack_.top();
        stack_.pop();

        stack_.push(EvalUnExpr(u.Op, e));
}

void Interpreter::Visit(Grouping& g) {
        g.Accept(*this);
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

TOut Interpreter::EvalBinExpr(Token t, TOut l, TOut r) {
    return std::visit(
        overload{
            [&r, &t](double a) -> TOut {
                return std::visit(
                    overload{
                        [&t, a](double b) -> TOut {
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
                        },
                        [](auto _) -> TOut { return TOut("Nil"); },
                    },
                    r);
            },
            [&r, &t](bool a) -> TOut {
                return std::visit(
                    overload{
                        [&t, a](bool b) -> TOut {
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
                            switch (t.Type) {
                                case TokenType::BANG_EQUAL:
                                    return s_l != s_r;
                                case TokenType::EQUAL_EQUAL:
                                    return s_l == s_r;
                                default:
                                    break;
                            }
                            RError(t, "Operator not supported for strings.");
                            return {"Nil"};
                        },
                        [&t](auto _) -> TOut {
                            RError(t,
                                   "Left and Right are not of the same type.");
                            return {"Nil"};
                        }},
                    r);
            }},
        l);
}

}  // namespace lox
