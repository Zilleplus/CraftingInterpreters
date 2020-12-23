#include "tokens.h"
#include <map>
#include <sstream>
#include <iostream>
#include "variantOverload.h"

namespace lox {

static std::map<TokenType, std::string> token_to_string {
    { TokenType::LEFT_PAREN, "LEFT_PAREN" },
    { TokenType::RIGHT_PAREN, "RIGHT_PAREN " },
    { TokenType::LEFT_BRACE, "LEFT_BRACE " },
    { TokenType::RIGHT_BRACE, "RIGHT_BRACE" },
    { TokenType::COMMA, "COMMA" },
    { TokenType::DOT, "DOT" },
    { TokenType::MINUS, "MINUS" },
    { TokenType::PLUS, "PLUS" },
    { TokenType::SEMICOLON, "SEMICOLON" },
    { TokenType::SLASH, "SLASHSTAR," },
    { TokenType::BANG, "BANG" },
    { TokenType::BANG_EQUAL, "BANG_EQUAL" },
    { TokenType::EQUAL, "EQUAL" },
    { TokenType::EQUAL_EQUAL, "EQUAL_EQUAL" },
    { TokenType::GREATER, "GREATER" },
    { TokenType::GREATER_EQUAL, "GREATER_EQUAL " },
    { TokenType::LESS, "LESS" },
    { TokenType::LESS_EQUAL, "LESS_EQUAL" },
    { TokenType::IDENTIFIER, "IDENTIFIER" },
    { TokenType::STRING, "STRING" },
    { TokenType::NUMBER, "NUMBER" },
    { TokenType::AND, "AND" },
    { TokenType::CLASS, "CLASS" },
    { TokenType::ELSE, "ELSE" },
    { TokenType::FALSE, "FALSE" },
    { TokenType::FUN, "FUN" },
    { TokenType::FOR, "FOR" },
    { TokenType::IF, "IF" },
    { TokenType::NIL, "NIL" },
    { TokenType::OR, "OR " },
    { TokenType::PRINT, "PRINT" },
    { TokenType::RETURN, "RETURN" },
    { TokenType::SUPER, "SUPER" },
    { TokenType::THIS, "THIS" },
    { TokenType::TRUE, "TRUE" },
    { TokenType::VAR, "VAR" },
    { TokenType::WHILE, "WHILE" },
    { TokenType::EOFL, "EOFL" }
};

inline std::string ToString(TokenType token_type)
{
    auto token_text = token_to_string.find(token_type);
    if(token_text == token_to_string.end())
    {
        return "no tostring for this token";
    }
    return token_text->second;
}

std::string Token::ToString()
{
    std::stringstream ss;
    auto data_string = std::visit(
            overload{
                [](double p){return std::to_string(p);},
                [](std::string p){return p;},
                [](std::monostate){return std::string("");}
                },
            this->data_);

    ss << lox::ToString(this->type_) << " lexeme:";
    ss << this->lexeme_ << " data:" << data_string;

    return ss.str();
}

}
