#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <filesystem>

#include"scanner.h"
#include"tokens.h"
#include "lox.h"

namespace lox {

static void runFile(const std::string& path)
{
    if(!std::filesystem::exists(path))
    {
        std::cerr << path << " does not exist.";
    }
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

int main(int argc, char* args[])
{
    if(argc>1)
    {
        std::string file_location_string = args[1];
        lox::runFile(file_location_string);
    }
    else{
        lox::runPrompt();
    }

}
