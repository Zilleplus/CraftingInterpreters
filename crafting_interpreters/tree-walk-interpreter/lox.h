#pragma once

#include <string>

namespace lox {

void Report(int line, std::string where, std::string message);
void Error(int line, std::string message);
void Run(const std::string& source);

}
