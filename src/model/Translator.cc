#include "Translator.h"
#include <iostream>
namespace s21 {
  list<std::string> Translator::Translate(const list<std::string>& tokens) const {

  }

  list<std::string> Translator::Tokenize(const std::string_view& expression) {
    list<std::string> tokens;
    pos_ = expression.begin();
    end_ = expression.end();
    prev_token_ = TokenType::UNKNOWN;
    current_token_ = TokenType::UNKNOWN;
    unclosed_ = bracket_ = 0;
    do {
      for (; pos_ != end_ && *pos_ == ' '; ++pos_) { }
      current_token_ = GetTokenType(pos_);
      TryOpenBracket(tokens);
      TryPlaceMultiply(tokens);
      ManageBrackets();
      if (isBracketsBroken())
        throw s21::bad_expression("Expression has an ambiguous bracket_s");
    } while (PushToken(tokens));
    if (bracket_ > 0 || unclosed_ > 0)
      PushBrackets(bracket_ + unclosed_, tokens);
    // if (!finishesExpr(GetTokenType(--tokens.end_())))
    //   throw s21::bad_expression("Expression is not finished");
    return tokens;
 }

  void Translator::TryOpenBracket(list<std::string>& dest) noexcept {
    if (prev_token_ == TokenType::FUNCTION && 
      (isNumeric(current_token_) || current_token_ == TokenType::OPERATOR)) {
      dest.push_back("(");
      ++unclosed_;
    } else if (isBracketFinisher(current_token_)) {
      PushBrackets(unclosed_, dest);
      unclosed_ = 0;
    }
  }

  void Translator::TryPlaceMultiply(list<std::string>& dest)  noexcept {
    if (skipsMultiplyRhs(prev_token_) && skipsMultiplyLhs(current_token_)) {
      dest.push_back("*");
    }
  }

  void Translator::ManageBrackets() noexcept {
    if (current_token_ == TokenType::OPEN_BRACKET) {
      ++bracket_;
    }
    else if (current_token_ == TokenType::CLOSE_BRACKET) {
      --bracket_;
    }
  }

  void Translator::PushBrackets(int count, list<std::string>& dest) noexcept {
    for (; count; --count)
        dest.push_back(")");
  }

  bool Translator::PushToken(list<std::string>& dest) noexcept
  {
    if (pos_ == end_)
      return false;
    if (current_token_ == TokenType::CLOSE_BRACKET && bracket_ < 0) {
      MovePos([this]() constexpr { return pos_ != end_ && *pos_ == ')';});
    } else if (isOneSymboled(current_token_)) {
      dest.push_back(std::string(pos_, pos_ + 1));
      ++pos_;
      prev_token_ = current_token_;
    } else if (current_token_ == TokenType::FUNCTION) {

    } else {
      position start = pos_;
      MovePos([this, start]() constexpr { return pos_ != end_ && GetTokenType(pos_) == TokenType::DIGIT;});
      dest.push_back(std::string(start, pos_));
      prev_token_ = current_token_;
    }
    return true;
  }

  template <typename Predicate>
  void Translator::MovePos(Predicate condition) noexcept {
      for (; condition(); ++pos_) { };
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

  constexpr bool Translator::isBracketFinisher(TokenType token) const noexcept {
    return token == TokenType::OPERATOR ||
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

  constexpr void Translator::MoveFunction() noexcept {
    if (std::string_view(pos_, 3).compare("sin"))
      pos_ += 3;
  }

  constexpr bool Translator::isBracketsBroken() const noexcept {
    return (prev_token_ == TokenType::OPERATOR || prev_token_ == TokenType::FUNCTION) 
           && current_token_ == TokenType::CLOSE_BRACKET;
  }
}
