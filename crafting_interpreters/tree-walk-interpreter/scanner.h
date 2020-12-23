#pragma once

#include<string>
#include<vector>
#include"tokens.h"
#include<variant>

namespace lox{



class Scanner{
        const std::string& source_;
        std::vector<Token> tokens_;

        int start_ = 0;
        int current_ = 0;
        int line_ = 1;

        bool IsAtEnd() const;
        bool IsAlpha(char) const;
        void ScanToken();
        char Advance();
        void AddToken(TokenType type);
        void AddToken(TokenType type, Token::TokenData literal);
        bool Match(char expected);
        char Peek() const ;
        char PeekNext() const;
        void string();
        void number();
        void Identifier();

    public:
        Scanner(const std::string& source);
        std::vector<Token>& ScanTokens();
};

}
