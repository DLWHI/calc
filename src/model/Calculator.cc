#include "Calculator.h"

#include <string_view>
#include <cmath>

#include "../containers/s21_stack.h"

namespace s21 {
  double Calculator::Calculate(const list<std::string>& expr, double x) {
    for (auto token: expr) {
      current_token_ = Tokenizer::GetTokenType(token.at(0));
      if (Tokenizer::IsNumeric(current_token_)) {
        calc_stack_.push(std::stod(token));
      } else if (Tokenizer::IsOperateable(current_token_)) {
        Operate(token.c_str());
      }
    }
    double result = StackPop();
    return result;
  }

  void Calculator::Operate(const std::string_view& operation) {
    if (kUnaryFunctions.contains(operation)) 
      calc_stack_.push(kUnaryFunctions[operation](StackPop()));
    else if (kBinaryFunctions.contains(operation)) 
      calc_stack_.push(kBinaryFunctions[operation](StackPop(), StackPop()));
  }

}  // namespace s21
