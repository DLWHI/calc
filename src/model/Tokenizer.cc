#include "Tokenizer.h"

namespace s21 {
  list<std::string> Tokenizer::Tokenize(const std::string_view& expression) {
    // Expression must not have any mod and pow words.
    // Controller should replace them with corresponding operators
    // before sending to tokenization. Tokenizer can only will be able
    // to work only with function representations of such operators.
    // Ex.: mod(5, 2), pow(2, 3)
    list<std::string> tokens;
    pos_ = expression.begin();
    end_ = expression.end();
    prev_token_ = TokenType::kUnknown;
    current_token_ = TokenType::kUnknown;
    for (; pos_ != end_ && *pos_ == ' '; ++pos_) { }
    do {
      push_ = State::kPush;
      current_token_ = GetTokenType(*pos_);
      Fix(tokens);
    } while (PushToken(tokens) && ValidState());
    ThrowErrors(tokens.back().c_str());
    for (; !brackets_.empty(); brackets_.pop())
      tokens.push_back(")");
    return tokens;
 }

  void Tokenizer::Fix(list<std::string>& dest) {
    if (ModCrutch(dest))
      return;
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

  bool Tokenizer::ModCrutch(list<std::string>& dest) {
    int rem = std::distance(pos_, end_);
    if (std::string_view("mod").compare(0, 3, pos_, std::min(3, rem))) {
      return false;
    }
    if (!IsNumeric(prev_token_) && prev_token_ != TokenType::kCloseBracket)
      push_ = State::kLonelyOperator;
    else
      dest.push_back("%");
    return true;
  }

  void Tokenizer::CloseBracket() noexcept {
    if (brackets_.empty()) {
      push_ = State::kDiscard;
    } else {
      brackets_.pop();
    }
  }

  void Tokenizer::CollapseOperator(list<std::string>& dest) {
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

  bool Tokenizer::PushToken(list<std::string>& dest)
  {
    position start = pos_;
    AdvancePosition();
    if (start == pos_)
      push_ = State::kFunctionErr;
    if (push_ == State::kPush) {
      dest.push_back(std::string(start, pos_));
      prev_token_ = current_token_;
    }
    for (; pos_ != end_ && *pos_ == ' '; ++pos_) { }
    return pos_ != end_;
  }

  char Tokenizer::OpUnary(const position& op) const noexcept {
    if (*op == '+' || *op == '#')
      return '#';
    else if (*op == '-' || *op == '~')
      return '~';
    return '\0';
  }

  char Tokenizer::OpBinary(const position& op) const noexcept {
    if (!IsNumeric(prev_token_) && prev_token_ != TokenType::kCloseBracket)
      return '\0';
    if (*op == '+' || *op == '#')
      return '+';
    else if (*op == '-' || *op == '~')
      return '-';
    return *op;
  }

  constexpr bool Tokenizer::ValidState() const noexcept {
    return push_ == State::kPush || push_ == State::kDiscard;
  }

  constexpr bool Tokenizer::MultiplySkipped() const noexcept {
    return (IsNumeric(prev_token_) || 
           prev_token_ == TokenType::kCloseBracket) &&
           (IsNumeric(current_token_) || 
           current_token_ == TokenType::kOpenBracket ||
           current_token_ == TokenType::kFunction);
  }

  constexpr bool Tokenizer::FunctionEmpty() const noexcept {
    return prev_token_ == TokenType::kFunction && 
           !IsNumeric(current_token_);
  }

  constexpr bool Tokenizer::BracketSkipped() const noexcept {
    return prev_token_ == TokenType::kFunction && 
      current_token_ != TokenType::kOpenBracket;
  }

  constexpr bool Tokenizer::BracketFinished() const noexcept {
    return !IsNumeric(current_token_) &&
           prev_token_ != TokenType::kFunction &&
           prev_token_ != TokenType::kOperator;
  }

  constexpr bool Tokenizer::BracketsBroken() const noexcept {
    return (prev_token_ == TokenType::kOperator || 
           prev_token_ == TokenType::kFunction || 
           prev_token_ == TokenType::kOpenBracket) &&
           current_token_ == TokenType::kCloseBracket;
  }

  constexpr bool Tokenizer::OneSymboled() const noexcept {
    return current_token_ == TokenType::kOperator ||
           current_token_ == TokenType::kArg ||
           current_token_ == TokenType::kOpenBracket ||
           current_token_ == TokenType::kCloseBracket;
  }
  
  constexpr bool Tokenizer::ExprFinished(TokenType last_token) const noexcept {
    return last_token == TokenType::kCloseBracket ||
           IsNumeric(last_token);
  }

  void Tokenizer::AdvancePosition() noexcept {
    if (OneSymboled()) {
      ++pos_;
    } else if (current_token_ == TokenType::kFunction) {
      std::size_t rem = std::distance(pos_, end_);
      auto func = kFunctions.begin();
      for (; func != kFunctions.end() && 
            func->compare(std::string_view(pos_, std::min(func->size(), rem))); ++func) { }
      pos_ += std::min(func->size(), rem)*(func != kFunctions.end());
    } else {
      for (; pos_ != end_ && (GetTokenType(*pos_) == TokenType::kDigit); ++pos_) { };
      if (pos_ != end_ && *pos_ == 'e') {
        ++pos_;
        if (pos_ != end_ && (*pos_ == '+' || *pos_ == '-'))
          ++pos_;
        for (; pos_ != end_ && (GetTokenType(*pos_) == TokenType::kDigit); ++pos_) { };
      }
    }
  }

  void Tokenizer::ThrowErrors(const std::string_view& last_token) const {
    if (push_ == State::kBrokenBrackets)
      throw std::logic_error("Expression has broken brackets");
    else if (push_ == State::kFunctionErr)
      throw std::logic_error("Expression has unknown function");
    else if (push_ == State::kLonelyOperator)
      throw std::logic_error("Expression has mismatched operator");
    else if (!ExprFinished(GetTokenType(*last_token.begin())))
      throw std::logic_error("Expression is not finished");
  }
}  // namespace s21
