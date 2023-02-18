#include "loxFunction.h"

#include "environment.h"
#include "interpreter.h"
#include "return.h"
#include <iostream>

namespace lox {

LoxFunction::TOut LoxFunction::Call(lox::Interpreter& interpreter,
                                    std::vector<TOut>& arguments) {
    for (int i = 0; i < std::size(declaration_.Params); ++i) {
        closure_->Define(declaration_.Params[i].Lexeme, arguments[i]);
    }

    try {
        interpreter.ExecuteBlock(declaration_.Body, closure_);
    } catch (Return r) {
        return r.Value;
    }
    return LoxFunction::TOut{"Nil"};
}

std::string LoxFunction::ToString() {
    return std::string("<fn ") + declaration_.Name.Lexeme + ">";
}

}  // namespace lox
