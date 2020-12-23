#include "scanner.h"
#include "lox.h"
#include <cctype>
#include <unordered_map>

namespace lox {

static std::unordered_map<std::string, TokenType> keywords {
  { "and", TokenType::AND },
  { "class", TokenType::CLASS },
  { "else", TokenType::ELSE },
  { "false", TokenType::FALSE },
  { "for", TokenType::FOR },
  { "fun", TokenType::FUN },
  { "if", TokenType::IF },
  { "nil", TokenType::NIL },
  { "or", TokenType::OR },
  { "print", TokenType::PRINT },
  { "return", TokenType::RETURN },
  { "super", TokenType::SUPER },
  { "this", TokenType::THIS },
  { "true", TokenType::TRUE },
  { "var", TokenType::VAR },
  { "while", TokenType::WHILE }
};

Scanner::Scanner(const std::string& source)
    : source_(source)
    , tokens_()
{
}

bool Scanner::IsAtEnd() const
{
  return this->current_ >= std::size(this->source_);
}

void Scanner::AddToken(TokenType type)
{
  AddToken(type, Token::TokenData());
}

void Scanner::AddToken(TokenType type, Token::TokenData data)
{
  auto text = source_.substr(start_, current_);
  tokens_.push_back(Token(type, text, data, line_));
}

char Scanner::Advance()
{
  current_++;
  return source_[current_ - 1];
}

char Scanner::Peek() const
{
  if (IsAtEnd())
    return '\0';
  return source_[current_];
}

char Scanner::PeekNext() const
{
  if (current_ + 1 >= std::size(source_))
    return '\0';

  return source_[current_ + 1];
}

void Scanner::ScanToken()
{
  auto c = Advance();

  switch (c) {
  case '(':
    AddToken(TokenType::LEFT_PAREN);
    break;
  case ')':
    AddToken(TokenType::RIGHT_PAREN);
    break;
  case '{':
    AddToken(TokenType::LEFT_BRACE);
    break;
  case '}':
    AddToken(TokenType::RIGHT_BRACE);
    break;
  case ',':
    AddToken(TokenType::COMMA);
    break;
  case '.':
    AddToken(TokenType::DOT);
    break;
  case '-':
    AddToken(TokenType::MINUS);
    break;
  case '+':
    AddToken(TokenType::PLUS);
    break;
  case ';':
    AddToken(TokenType::SEMICOLON);
    break;
  case '*':
    AddToken(TokenType::STAR);
    break;
  case '!':
    AddToken(Match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
    break;
  case '=':
    AddToken(Match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
    break;
  case '<':
    AddToken(Match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
    break;
  case '>':
    AddToken(Match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
    break;
  case '/':
    if (Match('/')) { // if this is comment...
      while (Peek() != '\n' && !IsAtEnd())
        Advance();
    } else {
      AddToken(TokenType::SLASH);
    }
    break;
  case ' ':
  case '\r':
  case '\t':
    break;
  case '\n':
    line_++;
    break;
  case '"':
    string();
    break;
  default:
    if (std::isdigit(c)) {
      number();
    } else if (std::isalpha(c)) {
      Identifier();
    } else {
      Error(line_, "unexpected char.");
    }
    break;
  }
}

void Scanner::string()
{
  while (Peek() != '"' && !IsAtEnd()) {
    if (Peek() == '\n')
      line_++;
    Advance();
  }

  if (IsAtEnd()) { // at the end but no closing '"'
    Error(line_, "Unterminated string");
    return;
  }

  // The closing '"'
  Advance();

  // Trim the surrouding quotes
  auto value = source_.substr(start_ + 1, current_ - 1);
  AddToken(TokenType::STRING, value);
}

std::vector<Token>& Scanner::ScanTokens()
{
  while (!IsAtEnd()) {
    this->start_ = this->current_;
    ScanToken();
  }

  tokens_.push_back(Token(TokenType::EOFL, "", Token::TokenData(), line_));

  return this->tokens_;
}

bool Scanner::Match(char expected)
{
  if (IsAtEnd())
    return false;
  if (source_[current_] != expected)
    return false;

  current_++;
  return true;
}

void Scanner::number()
{
  while (std::isdigit(Peek())) {
    Advance();
  }

  // Look for fractional part
  if (Peek() == '.' && std::isdigit(PeekNext())) {
    // Consume .
    Advance();

    while (std::isdigit(Peek())) {
      Advance();
    }
  }

  AddToken(
      TokenType::NUMBER,
      Token::TokenData(std::atof(source_.substr(start_, current_).c_str())));
}

bool Scanner::IsAlpha(char c) const
{
  return std::isalpha(c) || c == '_';
}

void Scanner::Identifier()
{
  while (std::isalpha(Peek()))
    Advance();

  auto token_type = keywords.find(source_.substr(start_, current_));
  if (token_type == keywords.end()) {
    AddToken(TokenType::IDENTIFIER);
  }
  else{
    AddToken(token_type->second);
  }
}

}
