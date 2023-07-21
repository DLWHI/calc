#ifndef SRC_MODEL_TRANSLATOR_H_
#define SRC_MODEL_TRANSLATOR_H_

#include "Tokenizer.h"
#include "../containers/s21_list.h"
#include "../containers/s21_stack.h"

namespace s21 {
class Translator final {
  public:
    typedef Tokenizer::TokenType TokenType;

    list<std::string> Translate(const list<std::string>& tokens);
  private:

    void PushToOut(list<std::string>& dest) {
      dest.push_back(std::string(op_stack_.top()));
      op_stack_.pop();
    }

    bool PriorityLess(const std::string& token) {
      return !op_stack_.empty() && 
      Tokenizer::GetPriority(token.at(0)) <= Tokenizer::GetPriority(op_stack_.top().at(0)) &&
      Tokenizer::IsLeftwise(token.at(0));
    };

    bool NotOpenBracket() {
      return !op_stack_.empty() && 
              Tokenizer::GetTokenType(op_stack_.top().at(0)) != TokenType::kOpenBracket;
    };

    bool FucntionOnTop() {
      return !op_stack_.empty() && 
             Tokenizer::GetTokenType(op_stack_.top().at(0)) == TokenType::kFunction;
    };

    stack<std::string> op_stack_;
    TokenType current_token_;
};
}  // namespace s21

#endif  // SRC_MODEL_TRANSLATOR_H_
