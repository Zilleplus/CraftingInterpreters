#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>
#include "syntaxTree.h"
#include "environment.h"

namespace lox {
class Interpreter;

class LoxFunction {
    using TOut =
        std::variant<bool, double, std::string, LoxFunction>;
    FunctionDeclaration declaration_;
    std::shared_ptr<Environment<TOut>> closure_;

   public:
    std::size_t Arity() const { return std::size(declaration_.Params); }

    LoxFunction(FunctionDeclaration decl, std::shared_ptr<Environment<TOut>>& closure) 
        : declaration_(decl), closure_(closure){}

    TOut Call(lox::Interpreter& interpreter, std::vector<TOut>& arguments);

    std::string ToString();
};
}  // namespace lox
