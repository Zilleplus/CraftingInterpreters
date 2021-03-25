#pragma once

#include"interpreter.h"

namespace lox
{
    class Return{
        using TOut = Interpreter::TOut;
        public:
        TOut Value;

        Return(TOut value) : Value(value){}
    };
}
