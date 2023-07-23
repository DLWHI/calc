#ifndef SRC_MODEL_CALCULATOR_H_
#define SRC_MODEL_CALCULATOR_H_
#include <cmath>
#include <stdexcept>

#include "Tokenizer.h"
#include "../containers/vector.h"
#include "../containers/s21_list.h"
#include "../containers/s21_stack.h"
#include "../containers/s21_map.h"

namespace s21 {
class Calculator final {
  public:
    Calculator() : kUnaryFunctions({
      std::pair("#", [](double x) { return x;}),
      std::pair("~", [](double x) { return -x;}),
      std::pair("ln", log),
      std::pair("tg", tan),
      std::pair("sin", sin),
      std::pair("cos", cos),
      std::pair("tan", tan),
      std::pair("ctg", [](double x) { return 1/tan(x);}),
      std::pair("cot", [](double x) { return 1/tan(x);}),
      std::pair("exp", exp),
      std::pair("log", log10),
      std::pair("atg", atan),
      std::pair("asin", asin),
      std::pair("acos", acos),
      std::pair("atan", atan),
      std::pair("acot", [](double x) { return M_PI_2 - atan(x);}),
      std::pair("actg", [](double x) { return M_PI_2 - atan(x);}),
      std::pair("sqrt", sqrt)
    }), kBinaryFunctions({
      std::pair("^", pow),
      std::pair("%", fmod),
      std::pair("+", [](double lhs, double rhs) { return lhs + rhs;}),
      std::pair("-", [](double lhs, double rhs) { return lhs - rhs;}),
      std::pair("*", [](double lhs, double rhs) { return lhs * rhs;}),
      std::pair("/", [](double lhs, double rhs) { return lhs / rhs;})
    }) { };

    typedef Tokenizer::TokenType TokenType;
    
    double Calculate(const list<std::string>& expr, double x = 0);

    vector<double> GenerateSet(double l, double r, int fin) const;
  private:
    map<std::string_view, std::function<double(double)>> kUnaryFunctions;
    map<std::string_view, std::function<double(double, double)>> kBinaryFunctions;

    void Operate(const std::string_view& operation);

    double StackPop() {
      double val = calc_stack_.top();
      calc_stack_.pop();
      return val;
    }

    double ToDouble(std::string src, double x) const;

    TokenType current_token_;
    stack<double> calc_stack_;
};
}  // namespace s21

#endif  // SRC_MODEL_CALCULATOR_H_
