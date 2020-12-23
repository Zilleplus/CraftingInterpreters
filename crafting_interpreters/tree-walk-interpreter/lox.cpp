#include "lox.h"
#include "scanner.h"
#include <iostream>

namespace lox{

static bool HadError = false;

void Report(int line, std::string where, std::string message){
    std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
    HadError = true;
}

void Error(int line, std::string message){
    Report(line, "", message);
}

void Run(const std::string& source)
{
    Scanner scanner(source);
    auto tokens = scanner.ScanTokens();

    for(auto t : tokens)
    {
        std::cout << t.ToString()<< std::endl;
    }
}

}
