#include "lox.h"

#include <iostream>

#include "interpreter.h"
#include "parser.h"
#include "scanner.h"

namespace lox {

static bool HadError = false;
static bool HadRunTimeError = false;

void Report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message
              << std::endl;
    HadError = true;
}

void Error(int line, std::string message) { Report(line, "", message); }

void ReportRunTimeError(RunTimeError re) {
    std::cerr << re.ErrorMsg << std::endl
              << "line[" << re.Operator.Line << "]" << std::endl;
    HadRunTimeError = true;
}

void Run(const std::string& source, bool debug_mode) {
    Scanner scanner(source);
    auto tokens = scanner.ScanTokens();
    Parser p(tokens);
    auto expr = p.Parse();

    if (HadError) {
        return;
    }

    Interpreter intp;
    intp.Interpret(*expr);
    if(debug_mode)
    {
        print(*expr);
    }
}

}  // namespace lox
