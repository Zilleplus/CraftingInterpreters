#pragma once

#include <string>
#include <variant>

namespace lox {

enum class TokenType {
    // single-char tokens.
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens.
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals.
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    EOFL
};

class Token final {
public:
    using TokenData = std::variant<
        std::monostate,
        std::string,
        double>;

    const TokenType Type;
    const std::string Lexeme;
    const TokenData Data;
    const int Line;

public:
    Token(
        const TokenType type,
        const std::string& lexeme,
        const TokenData data,
        const int line)
        : Type(type)
        , Lexeme(lexeme)
        , Data(std::move(data))
        , Line(line)
    {
    }

    std::string ToString();
};

std::string ToString(TokenType);

}
