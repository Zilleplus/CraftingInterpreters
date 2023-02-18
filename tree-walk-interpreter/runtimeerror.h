#pragma once

#include<string>
#include"syntaxTree.h"

namespace lox{

struct RunTimeError {
    Token Operator;
    std::string ErrorMsg;
};

}
