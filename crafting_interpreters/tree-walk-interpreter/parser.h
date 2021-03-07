#include "syntaxTree.h"
#include "tokens.h"
#include "lox.h"
#include <memory>
#include <vector>
#include <string>

namespace lox {

class Parser {
private:
    const std::vector<Token>& tokens_;
    int current_ = 0;

public:
    Parser(std::vector<Token>& tokens)
        : tokens_(tokens)
    {
    }
    
    std::unique_ptr<Expression> Parse(){
        try{
            return Expr();
        }
        catch (ParseError e){
            // no recovery at this moment
            return nullptr;
        }
    }

private:
    struct ParseError{}; 

    // Consume the current token, and return it.
    Token Advance();

    // Return the next token.
    Token Peek() const;

    // Return the previous token, aka the last consumed token.
    Token Previous() const;

    // Check if the next token type equals some type.
    bool Check(TokenType type) const;

    // Check if their are more token to be consumed.
    bool IsAtEnd() const;

    static void ReportError(Token token, std::string&& message)
    {
        if(token.Type == TokenType::EOFL)
        {
            Report(token.Line, " at end", std::move(message));
        }
        else{
            std::string err_msg = " at ";
            err_msg.append(token.Lexeme);

            Report(token.Line, std::move(err_msg), std::move(message));
        }
    }

    ParseError Error(Token token, std::string&& message)
    {
        ReportError(token, std::move(message));
        return ParseError();
    }

    std::unique_ptr<Expression> Expr();

    std::unique_ptr<Expression> Equality();

    std::unique_ptr<Expression> Comparison();

    std::unique_ptr<Expression> Term();

    std::unique_ptr<Expression> Factor();

    std::unique_ptr<Expression> Unary();

    std::unique_ptr<Expression> Primary();

    Token Consume(TokenType type, std::string&& message)
    {
        if(Check(type)) 
        {
            return Advance();
        }

        throw Error(Peek(), std::move(message));
    }

    // Check if the next token equals one of the provided token types.
    // If it matches it returns true, and consume the token. Otherwise 
    // return false.
    bool Match(std::initializer_list<TokenType> tokens);
    
    void Synchronize();
};

}
