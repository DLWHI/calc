#include "Translator.h"
#include <iostream>
namespace s21 {
  vector<std::string> Translator::Translate(const std::string& expression) const {

  }

  void Translator::Tokenize(const std::string& expression) const {

  }

std::string Translator::Fix(const std::string& expression) const {
    std::string fixed;
    TokenType prev_token = TokenType::UNKNOWN;
    TokenType current_token = TokenType::UNKNOWN;
    int unclosed = 0, bracket = 0;
    for (position i = expression.begin(); i != expression.end(); PushToken(i, fixed)) {
      for (; *i == ' '; i++) { }
      current_token = GetTokenType(i);
      if (skipsMultiplyRhs(prev_token) && skipsMultiplyLhs(current_token)) {
        fixed += '*';
      }
      prev_token = current_token;
    }
    // if (!finishesExpr(last_token))
    //   throw s21::bad_expression("Expression is not finished");
    if (bracket < 0) 
      return fixed;  // add parenthesis fixing
    else if (bracket > 0 || unclosed > 0)  
      PushBrackets(bracket + unclosed, fixed);
    return fixed;
  }

  int Translator::stateNumeric(std::string& dest, TokenType prev) const noexcept {
    if (prev == TokenType::FUNCTION) {
      dest += '(';
      return 1;
    }
  }

  int Translator::stateOperator(std::string& dest, TokenType prev) const noexcept {
    if (prev == TokenType::FUNCTION) {
      dest += '(';
      return 1;
    }
    // for each unclosed bracket added by fixer, add it
  }

  int Translator::stateFunction(std::string& dest, TokenType prev) const noexcept {
    // for each unclosed bracket added by fixer, add it
  }

  int Translator::stateBra(std::string& dest, TokenType prev) const noexcept {
    // for each unclosed bracket added by fixer, add it
  }

  int Translator::stateKet(std::string& dest, TokenType prev) const noexcept {
    if (!isNumeric(prev) && prev != TokenType::CLOSE_BRACKET) {
      return 0;  // discard
    }
  }

  void Translator::PushBrackets(int count, std::string& dest) const noexcept {
    for (; count; --count)
        dest += ')';
  }

  void Translator::PushToken(position& pos, std::string& dest) const noexcept
  {
    TokenType current_type = GetTokenType(pos);
    dest += *pos++;
    for (; current_type == GetTokenType(pos) && !isOneSymboled(current_type); pos++)
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

  constexpr bool Translator::skipsMultiplyRhs(TokenType token) const {
    return isNumeric(token) || 
           token == TokenType::CLOSE_BRACKET;
  }

  constexpr bool Translator::skipsMultiplyLhs(TokenType token) const {
    return isNumeric(token) || 
           token == TokenType::OPEN_BRACKET ||
           token == TokenType::FUNCTION;
  }

  constexpr bool Translator::isNumeric(TokenType token) const
  {
    return token == TokenType::DIGIT ||
           token == TokenType::ARG;
  }
  
  constexpr bool Translator::isOneSymboled(TokenType token) const
  {
    return token == TokenType::OPERATOR ||
           token == TokenType::ARG ||
           token == TokenType::OPEN_BRACKET ||
           token == TokenType::CLOSE_BRACKET;
  }

  constexpr bool Translator::finishesExpr(TokenType token) const {
    return token == TokenType::DIGIT ||
           token == TokenType::ARG ||
           token == TokenType::CLOSE_BRACKET;
  }
}
