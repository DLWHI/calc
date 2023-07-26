#include "Calculator.h"

#include <string>
#include <string_view>

#include "../containers/s21_stack.h"

namespace s21 {
  double Calculator::Calculate(const list<std::string>& expr, double x) {
    for (auto token: expr) {
      current_token_ = Tokenizer::GetTokenType(token.at(0));
      if (Tokenizer::IsNumeric(current_token_)) {
        calc_stack_.push(ToDouble(token, x));
      } else if (Tokenizer::IsOperateable(current_token_)) {
        Operate(token.c_str());
      }
    }
    double result = StackPop();
    return result;
  }

  void Calculator::Operate(const std::string_view& operation) {
    double r = StackPop();
    if (kUnaryFunctions.contains(operation)) 
      return calc_stack_.push(kUnaryFunctions[operation](r));
    double l = StackPop();
    if (kBinaryFunctions.contains(operation)) 
      return calc_stack_.push(kBinaryFunctions[operation](l, r));
  }

  double Calculator::ToDouble(std::string src, double x) const {
    if (src.at(0) == 'x')
      return x;
    std::size_t idx;
    double dbl = std::stod(src, &idx);
    if (idx != src.size())
      throw std::invalid_argument("Invalid number in expression");
    return dbl;
  };

  vector<double> Calculator::GenerateSet(double l, double r, int fin) const {
    if (l > r)
      throw std::domain_error("Invalid set boundaries");
    vector<double> points;
    points.reserve(fin);
    double delta = (r - l)/fin;
    for (; l < r; l += delta)
      points.push_back(l);
    return points;
  }
}  // namespace s21
