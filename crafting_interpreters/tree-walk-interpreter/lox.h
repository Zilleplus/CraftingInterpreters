#pragma once

#include <string>

namespace lox {

struct RunTimeError;

void Report(int line, std::string where, std::string message);
void ReportRunTimeError(RunTimeError re);
void Error(int line, std::string message);
void Run(const std::string& source, bool debug_mode = false);

}
