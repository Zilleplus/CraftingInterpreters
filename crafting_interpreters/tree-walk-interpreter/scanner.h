#pragma once

#include <string>
#include <variant>
#include <vector>

#include "tokens.h"

namespace lox {

class Scanner {
    const std::string& source_;
    std::vector<Token> tokens_;

    int start_ = 0;
    int current_ = 0;
    int line_ = 1;

    bool IsAtEnd() const;
    static bool IsAlpha(char);
    void ScanToken();
    char Advance();
    void AddToken(TokenType type);
    void AddToken(TokenType type, Token::TokenData data);
    bool Match(char expected);
    char Peek() const;
    char PeekNext() const;
    void string();
    void number();
    void Identifier();

   public:
    Scanner(const std::string& source);
    std::vector<Token>& ScanTokens();
};

}  // namespace lox
