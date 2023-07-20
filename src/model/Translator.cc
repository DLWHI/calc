#include "Translator.h"

namespace s21 {
  list<std::string> Translator::Translate(const list<std::string>& tokens) {
    list<std::string> postfix;
    for (auto token: tokens) {
      current_token_ = Tokenizer::GetTokenType(token.at(0));
      if (Tokenizer::IsNumeric(current_token_)) {
        postfix.push_back(token);
      } else if (current_token_ == TokenType::kFunction || current_token_ == TokenType::kOpenBracket) {
        op_stack_.push(token);
      } else if (current_token_ == TokenType::kOperator) {
        while (PriorityLess(token))
          PushToOut(postfix);
        op_stack_.push(token);
      } else if (current_token_ == TokenType::kCloseBracket) {
        while (NotOpenBracket())
          PushToOut(postfix);
        op_stack_.pop();
        if (FucntionOnTop())
          PushToOut(postfix);
      }
    }
    while (!op_stack_.empty()) {
      PushToOut(postfix);
    }
    return postfix;
  }
}  // namespace s21
