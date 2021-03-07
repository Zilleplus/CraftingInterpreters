#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

#include"scanner.h"
#include"tokens.h"
#include "lox.h"

namespace lox {

static void runFile(const std::string& path)
{
    std::ifstream t(path);
    Run(std::string(
        std::istreambuf_iterator<char>(t),
        std::istreambuf_iterator<char>()));
}

static void runPrompt()
{
    for (;;) {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
            Run(line);
    }
}

}

int main()
{
    lox::runPrompt();
}
