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
    do {
      push_ = State::kPush;
      for (; pos_ != end_ && *pos_ == ' '; ++pos_) { }
      current_token_ = GetTokenType(pos_);
      Fix(tokens);
    } while (PushToken(tokens) && ValidState());
    ThrowErrors(tokens.back().c_str());
    for (; !brackets_.empty(); brackets_.pop())
      tokens.push_back(")");
    return tokens;
 }

  void Translator::Fix(list<std::string>& dest) noexcept {
    if (BracketSkipped()) {
      dest.push_back("(");
      brackets_.push(-')');
    } else if (BracketFinished()) {
      for (; !brackets_.empty() && brackets_.top() < 0; brackets_.pop())
        dest.push_back(")");
    }

    if (current_token_ == TokenType::kOperator) {
      CollapseOperator(dest);
    } else if (current_token_ == TokenType::kOpenBracket) {
      brackets_.push(')');
    }
    else if (current_token_ == TokenType::kCloseBracket) {
      CloseBracket();
    }

    if (MultiplySkipped()) {
      dest.push_back("*");
    } else if (BracketsBroken()) {
      push_ = State::kBrokenBrackets;
    }
  }

  void Translator::CloseBracket() noexcept {
    if (brackets_.empty()) {
      push_ = State::kDiscard;
    } else {
      brackets_.pop();
    }
  }

  void Translator::CollapseOperator(list<std::string>& dest) noexcept {
    push_ = State::kDiscard;
    if (OpBinary(pos_)) {
        dest.push_back(std::string(1, OpBinary(pos_)));
    } else if (OpUnary(pos_)) {
        dest.push_back(std::string(1, OpUnary(pos_)));
    } else {
      push_ = State::kLonelyOperator;
    }
    prev_token_ = current_token_;
  }

  bool Translator::PushToken(list<std::string>& dest) noexcept
  {
    position start = pos_;
    AdvancePosition();
    if (start == pos_)
      push_ = State::kFunctionErr;
    if (push_ == State::kPush) {
      dest.push_back(std::string(start, pos_));
      prev_token_ = current_token_;
    }
    return pos_ != end_;
  }

  Translator::TokenType Translator::GetTokenType(const position& symbol) const noexcept
  {
    if (('0' <= *symbol && *symbol <= '9') || *symbol == '.')
      return TokenType::kDigit;
    else if (kOperators.find(*symbol) + 1)
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

  char Translator::OpUnary(const position& op) const noexcept {
    if (*op == '+' || *op == '#')
      return '#';
    else if (*op == '-' || *op == '~')
      return '~';
    return '\0';
  }

  char Translator::OpBinary(const position& op) const noexcept {
    if (!IsNumeric(prev_token_) && prev_token_ != TokenType::kCloseBracket)
      return '\0';
    if (*op == '+' || *op == '#')
      return '+';
    else if (*op == '-' || *op == '~')
      return '-';
    return *op;
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
    return !IsNumeric(current_token_) &&
           prev_token_ != TokenType::kFunction &&
           prev_token_ != TokenType::kOperator;
  }

  constexpr bool Translator::BracketsBroken() const noexcept {
    return (prev_token_ == TokenType::kOperator || 
           prev_token_ == TokenType::kFunction || 
           prev_token_ == TokenType::kOpenBracket) &&
           current_token_ == TokenType::kCloseBracket;
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
  
  constexpr bool Translator::ExprFinished(TokenType last_token) const noexcept {
    return last_token == TokenType::kCloseBracket ||
           IsNumeric(last_token);
  }

  void Translator::AdvancePosition() noexcept {
    if (OneSymboled()) {
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
      throw std::logic_error("Expression has broken brackets");
    else if (push_ == State::kFunctionErr)
      throw std::logic_error("Expression unknown function");
    else if (push_ == State::kLonelyOperator)
      throw std::logic_error("Expression has mismatched operator");
    else if (!ExprFinished(GetTokenType(last_token.begin())))
      throw std::logic_error("Expression is not finished");
  }
}
