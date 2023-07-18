#include "Translator.h"
#include <iostream>
namespace s21 {
  list<std::string> Translator::Translate(const list<std::string>& tokens) const {

  }

  list<std::string> Translator::Tokenize(const std::string_view& expression) {
    list<std::string> tokens;
    pos_ = expression.begin();
    end_ = expression.end();
    prev_token_ = TokenType::kUnknown;
    current_token_ = TokenType::kUnknown;
    unclosed_ = bracket_ = 0;
    do {
      push_ = State::kPush;
      for (; pos_ != end_ && *pos_ == ' '; ++pos_) { }
      current_token_ = GetTokenType(pos_);
      Fix(tokens);
    } while (PushToken(tokens) && ValidState());
    ThrowErrors(tokens.back().c_str());
    if (bracket_ + unclosed_ > 0)
      PushBrackets(bracket_ + unclosed_, tokens);
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

    if (current_token_ == TokenType::kOperator && !IsNumeric(prev_token_)) {
      push_ = State::kDiscard;
      if (*pos_ == '+')
        dest.push_back("#");
      else if (*pos_ == '-')
        dest.push_back("~");
      else
        push_ = State::kLonelyOperator;
      prev_token_ = current_token_;
    } else if (current_token_ == TokenType::kOpenBracket) {
      ++bracket_;
    }
    else if (current_token_ == TokenType::kCloseBracket) {
      --bracket_;
    }

    if (MultiplySkipped()) {
      dest.push_back("*");
    } else if (BracketsBroken()) {
      push_ = State::kBrokenBrackets;
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
    position start = pos_;
    AdvancePosition();
    if (start == pos_)
      push_ = State::kFunctionErr;
    if (push_ == State::kPush) {
      dest.push_back(std::string(start, pos_));
      prev_token_ = current_token_;
    }
    return true;
  }

  Translator::TokenType Translator::GetTokenType(const position& symbol) const noexcept
  {
    if (('0' <= *symbol && *symbol <= '9') || *symbol == '.')
      return TokenType::kDigit;
    else if (kOperators.find(*symbol) != std::string_view::npos)
      return TokenType::kOperator;
    else if (*symbol == '(')
      return TokenType::kOpenBracket;
    else if (*symbol == ')')
      return TokenType::kCloseBracket;
    else if (*symbol == 'x' || *symbol == 'X' || *symbol == 'y' || *symbol == 'Y')
      return TokenType::kArg;
    else if (*symbol == ' ' || !*symbol)
      return TokenType::kUnknown;
    return TokenType::kFunction;
  }

  constexpr bool Translator::ValidState() const noexcept {
    return push_ == State::kPush || push_ == State::kDiscard;
  }

  constexpr bool Translator::MultiplySkipped() const noexcept {
    return (IsNumeric(prev_token_) || 
           prev_token_ == TokenType::kCloseBracket) &&
           (IsNumeric(current_token_) || 
           current_token_ == TokenType::kOpenBracket ||
           current_token_ == TokenType::kFunction);
  }

  constexpr bool Translator::FunctionEmpty() const noexcept {
    return prev_token_ == TokenType::kFunction && 
           !IsNumeric(current_token_);
  }

  constexpr bool Translator::BracketSkipped() const noexcept {
    return prev_token_ == TokenType::kFunction && 
      current_token_ != TokenType::kOpenBracket;
  }

  constexpr bool Translator::BracketFinished() const noexcept {
    return (current_token_ == TokenType::kOperator ||
           current_token_ == TokenType::kOpenBracket ||
           current_token_ == TokenType::kFunction) &&
           prev_token_ != TokenType::kFunction && 
           (unclosed_ + bracket_) == 1;
  }

  constexpr bool Translator::BracketNotOpened() const noexcept {
    return current_token_ == TokenType::kCloseBracket && bracket_ < 0;
  }

  constexpr bool Translator::BracketsBroken() const noexcept {
    return (prev_token_ == TokenType::kOperator || prev_token_ == TokenType::kFunction) 
           && current_token_ == TokenType::kCloseBracket;
  }

  constexpr bool Translator::OneSymboled() const noexcept {
    return current_token_ == TokenType::kOperator ||
           current_token_ == TokenType::kArg ||
           current_token_ == TokenType::kOpenBracket ||
           current_token_ == TokenType::kCloseBracket;
  }
  
  constexpr bool Translator::IsNumeric(TokenType token) const noexcept {
    return token == TokenType::kDigit ||
           token == TokenType::kArg;
  }
  
  bool Translator::ExprFinished(const std::string_view& last_token) const noexcept {
    return kOperators.find(last_token) != std::string_view::npos || last_token.compare("(");
  }

  void Translator::AdvancePosition() noexcept {
    if (bracket_ < 0) {
      for (; pos_ != end_ && *pos_ == ')'; ++pos_) { };
      push_ = State::kDiscard;
      bracket_ = 0;
    } else if (OneSymboled()) {
      ++pos_;
    } else if (current_token_ == TokenType::kFunction) {
      std::size_t rem = std::distance(pos_, end_);
      auto func = kFunctions.begin();
      for (; func != kFunctions.end() && 
            func->compare(std::string_view(pos_, std::min(func->size(), rem))); ++func) { }
      pos_ += std::min(func->size(), rem);
    } else {
      for (; pos_ != end_ && GetTokenType(pos_) == TokenType::kDigit; ++pos_) { };
    }
  }

  void Translator::ThrowErrors(const std::string_view& last_token) const {
    if (push_ == State::kBrokenBrackets)
      throw bad_expression("Expression has broken brackets");
    else if (push_ == State::kFunctionErr)
      throw bad_expression("Expression unknown function");
    else if (push_ == State::kLonelyOperator)
      throw bad_expression("Expression has mismatched operator");
    else if (!ExprFinished(last_token))
      throw bad_expression("Expression is not finished");
  }
}
