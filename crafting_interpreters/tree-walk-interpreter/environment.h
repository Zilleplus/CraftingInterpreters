#pragma once

#include <map>
#include <string>
#include <variant>

#include "runtimeerror.h"
#include "syntaxTree.h"

namespace lox {

template <typename TOut>
class Environment {
   public:
    using ValueType = TOut;
    std::unique_ptr<Environment<ValueType>> enclosing = nullptr;

   private:
    std::map<std::string, ValueType> values;

   public:
    Environment();
    Environment(std::unique_ptr<Environment<ValueType>>&& env);

    void Define(const std::string& name,
                typename Environment<TOut>::ValueType value);
    void Assign(const Token& name, typename Environment<TOut>::ValueType value);
    ValueType Get(Token name);
};

template <typename T>
Environment<T>::Environment() {}

template <typename T>
Environment<T>::Environment(std::unique_ptr<Environment<T>>&& env)
    : enclosing(std::move(env)) {}

template <typename T>
void Environment<T>::Define(const std::string& name,
                            typename Environment<T>::ValueType value) {
    this->values.insert_or_assign(name, value);
}

template <typename T>
void Environment<T>::Assign(const Token& name,
                            typename Environment<T>::ValueType value) {
    auto v = values.find(name.Lexeme);
    if (v != values.end()) {
        v->second = value;
    } else {
        if (enclosing != nullptr) {
            // operation throws...
            enclosing->Assign(name, value);
            return;
        }
        auto err_msg = std::string("Undefined variable'") + name.Lexeme +
                       std::string("'.");
        throw RunTimeError{name, err_msg};
    }
}

template <typename T>
typename Environment<T>::ValueType Environment<T>::Get(Token name) {
    auto val = values.find(std::get<std::string>(name.Data));

    if (val == values.end()) {
        if (enclosing != nullptr)  // if not found here, check the enclosing
        {
            // This operation throws...
            return enclosing->Get(name);
        }

        throw RunTimeError{name, std::string("Undefined variable '") +
                                     std::get<std::string>(name.Data) +
                                     std::string("'.")};
    }

    return val->second;
}

}  // namespace lox
