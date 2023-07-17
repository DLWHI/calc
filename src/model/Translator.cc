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
      Fix(tokens);
      if (BracketsBroken())
        throw s21::bad_expression("Expression has an ambiguous brackets");
    } while (PushToken(tokens));
    if (bracket_ + unclosed_ > 0)
      PushBrackets(bracket_ + unclosed_, tokens);
    if (!ExprFinished(tokens.back()))
      throw s21::bad_expression("Expression is not finished");
    return tokens;
 }

  void Translator::Fix(list<std::string>& dest) noexcept {
    if (BracketSkipped()) {
      dest.push_back("(");
      ++unclosed_;
    } else if (BracketFinished()) {
      PushBrackets(unclosed_, dest);
      unclosed_ = 0;
    }

    if (current_token_ == TokenType::OPEN_BRACKET) {
      ++bracket_;
    }
    else if (current_token_ == TokenType::CLOSE_BRACKET) {
      --bracket_;
    }

    if (MultiplySkipped()) {
      dest.push_back("*");
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
    if (BracketNotOpened()) {
      for (; pos_ != end_ && *pos_ == ')'; ++pos_) { };
      bracket_ = 0;
    } else {
      position start = pos_;
      AdvancePosition();
      dest.push_back(std::string(start, pos_));
      prev_token_ = current_token_;
    }
    return true;
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

  constexpr bool Translator::MultiplySkipped() const noexcept {
    return (IsNumeric(prev_token_) || 
           prev_token_ == TokenType::CLOSE_BRACKET) &&
           (IsNumeric(current_token_) || 
           current_token_ == TokenType::OPEN_BRACKET ||
           current_token_ == TokenType::FUNCTION);
  }

  constexpr bool Translator::BracketSkipped() const noexcept {
    return prev_token_ == TokenType::FUNCTION && 
      current_token_ != TokenType::OPEN_BRACKET;
  }

  constexpr bool Translator::BracketFinished() const noexcept {
    return (current_token_ == TokenType::OPERATOR ||
           current_token_ == TokenType::OPEN_BRACKET ||
           current_token_ == TokenType::FUNCTION) &&
           prev_token_ != TokenType::FUNCTION && 
           (unclosed_ + bracket_) == 1;
  }

  constexpr bool Translator::BracketNotOpened() const noexcept {
    return current_token_ == TokenType::CLOSE_BRACKET && bracket_ < 0;
  }

  constexpr bool Translator::BracketsBroken() const noexcept {
    return (prev_token_ == TokenType::OPERATOR || prev_token_ == TokenType::FUNCTION) 
           && current_token_ == TokenType::CLOSE_BRACKET;
  }

  constexpr bool Translator::OneSymboled() const noexcept {
    return current_token_ == TokenType::OPERATOR ||
           current_token_ == TokenType::ARG ||
           current_token_ == TokenType::OPEN_BRACKET ||
           current_token_ == TokenType::CLOSE_BRACKET;
  }
  
  constexpr bool Translator::IsNumeric(TokenType token) const noexcept {
    return token == TokenType::DIGIT ||
           token == TokenType::ARG;
  }
  
  bool Translator::ExprFinished(const std::string& last_token) const noexcept {
    return kOperators.find(last_token.c_str()) == std::string_view::npos;
  }

  void Translator::AdvancePosition() noexcept {
    if (OneSymboled()) {
        ++pos_;
    } else if (current_token_ == TokenType::FUNCTION) {
      std::size_t rem = std::distance(pos_, end_);
      auto func = kFunctions.begin();
      for (; func != kFunctions.end() && 
            func->compare(std::string_view(pos_, std::min(func->size(), rem))); ++func) { }
      pos_ += std::min(func->size(), rem);
    } else {
      for (; pos_ != end_ && GetTokenType(pos_) == TokenType::DIGIT; ++pos_) { };
    }
  }
}
