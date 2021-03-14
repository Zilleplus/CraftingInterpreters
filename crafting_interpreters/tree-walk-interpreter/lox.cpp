#include "lox.h"

#include <iostream>

#include "interpreter.h"
#include "parser.h"
#include "scanner.h"

namespace lox {

static bool HadError = false;
static bool HadRunTimeError = false;
static Interpreter intp;

void Report(int line, std::string where, std::string message) {
    std::cout << "[line " << line << "] Error" << where << ": " << message
              << std::endl;
    HadError = true;
}

void Error(int line, std::string message) { Report(line, "", message); }

void ReportRunTimeError(RunTimeError re) {
    std::cout << re.ErrorMsg << std::endl
              << "line[" << re.Operator.Line << "]" << std::endl;
    HadRunTimeError = true;
}

void Run(const std::string& source) {
    Scanner scanner(source);
    auto tokens = scanner.ScanTokens();
    Parser p(tokens);
    auto statements = p.Parse();

    if (HadError) {
        HadError=false;
        return;
    }

    intp.Interpret(statements);
}

}  // namespace lox
