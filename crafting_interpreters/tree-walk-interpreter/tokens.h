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

private:
    const TokenType type_;
    const std::string lexeme_;
    const TokenData data_;
    const int line_;

public:
    Token(
        const TokenType type,
        const std::string& lexeme,
        const TokenData data,
        const int line)
        : type_(type)
        , lexeme_(lexeme)
        , data_(std::move(data))
        , line_(line)
    {
    }

    std::string ToString();
};

}
