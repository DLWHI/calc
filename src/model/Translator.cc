#include "Translator.h"
#include <iostream>
namespace s21 {
  vector<std::string> Translator::Translate(const std::string& expression) const {

  }

  void Translator::Tokenize(const std::string& expression) const {

  }

std::string Translator::Fix(const std::string& expression) {
    std::string fixed;
    pos = expression.begin();
    end = expression.end();
    prev_token = TokenType::UNKNOWN;
    current_token = TokenType::UNKNOWN;
    unclosed = bracket = 0;
    for (; pos != end; PushToken(fixed)) {
      for (; pos != end && *pos == ' '; ++pos) { }
      current_token = GetTokenType(pos);
      TryOpenBracket(fixed);
      if (skipsMultiplyRhs(prev_token) && skipsMultiplyLhs(current_token)) {
        fixed += '*';
      }
      ManageBrackets();
      prev_token = current_token;
    }
    // if (!finishesExpr(prev_token))
    //   throw s21::bad_expression("Expression is not finished");
    if (bracket > 0 || unclosed > 0)  
      PushBrackets(bracket + unclosed, fixed);
    return fixed;
  }

  void Translator::TryOpenBracket(std::string& dest) noexcept {
    if (prev_token == TokenType::FUNCTION && 
      (isNumeric(current_token) || current_token == TokenType::OPERATOR)) {
      dest += '(';
      ++unclosed;
    } else if (current_token == TokenType::OPERATOR ||
        current_token == TokenType::OPEN_BRACKET ||
        current_token == TokenType::FUNCTION) {
      PushBrackets(unclosed, dest);
      unclosed = 0;
    }
  }

  void Translator::ManageBrackets() {
    if (current_token == TokenType::OPEN_BRACKET) {
      ++bracket;
    }
    else if (current_token == TokenType::CLOSE_BRACKET) {
      --bracket;
      if (bracket < 0 && pos != end) {
        for (bracket = 0; pos != end && *pos == ')'; ++pos) { };
        current_token = GetTokenType(pos);
      }
      if (prev_token == TokenType::OPERATOR || prev_token == TokenType::FUNCTION)
        throw s21::bad_expression("Expression has an ambiguous brackets");
    }
  }

  void Translator::PushBrackets(int count, std::string& dest) noexcept {
    for (; count; --count)
        dest += ')';
  }

  void Translator::PushToken(std::string& dest) noexcept
  {
    if (pos != end)
      dest += *pos++;
    if (!isOneSymboled(prev_token))
      for (; pos != end && prev_token == GetTokenType(pos); pos++)
        dest += *pos;
  }

  Translator::TokenType Translator::GetTokenType(const position& symbol) const noexcept
  {
    if (('0' <= *symbol && *symbol <= '9') || *symbol == '.')
      return TokenType::DIGIT;
    else if (kOperators.find(*symbol) != std::string_view::npos)
      return TokenType::OPERATOR;
    else if (*symbol == '(')
      return TokenType::OPEN_BRACKET;
    else if (*symbol == ')')
      return TokenType::CLOSE_BRACKET;
    else if (*symbol == 'x' || *symbol == 'X' || *symbol == 'y' || *symbol == 'Y')
      return TokenType::ARG;
    else if (*symbol == ' ' || !*symbol)
      return TokenType::UNKNOWN;
    return TokenType::FUNCTION;
  }

  constexpr bool Translator::skipsMultiplyRhs(TokenType token) const noexcept {
    return isNumeric(token) || 
           token == TokenType::CLOSE_BRACKET;
  }

  constexpr bool Translator::skipsMultiplyLhs(TokenType token) const noexcept {
    return isNumeric(token) || 
           token == TokenType::OPEN_BRACKET ||
           token == TokenType::FUNCTION;
  }

  constexpr bool Translator::isNumeric(TokenType token) const noexcept {
    return token == TokenType::DIGIT ||
           token == TokenType::ARG;
  }
  
  constexpr bool Translator::isOneSymboled(TokenType token) const noexcept {
    return token == TokenType::OPERATOR ||
           token == TokenType::ARG ||
           token == TokenType::OPEN_BRACKET ||
           token == TokenType::CLOSE_BRACKET;
  }

  constexpr bool Translator::finishesExpr(TokenType token) const noexcept {
    return token == TokenType::DIGIT ||
           token == TokenType::ARG ||
           token == TokenType::CLOSE_BRACKET;
  }
}
