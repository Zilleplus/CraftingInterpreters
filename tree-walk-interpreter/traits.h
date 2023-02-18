#pragma once

namespace lox{

    template<typename T>
    struct DataType;

    template<>
    struct DataType<Interpreter>
    {
        using Type = Interpreter::TOut;
    };

}
