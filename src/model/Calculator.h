#ifndef SRC_MODEL_CALCULATOR_H_
#define SRC_MODEL_CALCULATOR_H_
#include <s21_list.h>
#include <s21_map.h>
#include <vector.h>

#include <cmath>
#include <functional>
#include <stdexcept>
#include <utility>

#include "tokenizer.h"

namespace s21 {
class Calculator final {
 public:
  Calculator()
      : kUnaryFunctions(
            {std::make_pair("#", [](double x) { return x; }),
             std::make_pair("~", [](double x) { return -x; }),
             std::make_pair("ln", [](double x) { return log(x); }),
             std::make_pair("tg", [](double x) { return tan(x); }),
             std::make_pair("sin", [](double x) { return sin(x); }),
             std::make_pair("cos", [](double x) { return cos(x); }),
             std::make_pair("tan", [](double x) { return tan(x); }),
             std::make_pair("ctg", [](double x) { return 1 / tan(x); }),
             std::make_pair("cot", [](double x) { return 1 / tan(x); }),
             std::make_pair("exp", [](double x) { return exp(x); }),
             std::make_pair("log", [](double x) { return log10(x); }),
             std::make_pair("atg", [](double x) { return atan(x); }),
             std::make_pair("asin", [](double x) { return asin(x); }),
             std::make_pair("acos", [](double x) { return acos(x); }),
             std::make_pair("atan", [](double x) { return atan(x); }),
             std::make_pair(
                 "acot",
                 [](double x) { return 1.57079632679489661923 - atan(x); }),
             std::make_pair(
                 "actg",
                 [](double x) { return 1.57079632679489661923 - atan(x); }),
             std::make_pair("sqrt", [](double x) { return sqrt(x); })}),
        kBinaryFunctions(
            {std::make_pair(
                 "^", [](double lhs, double rhs) { return pow(lhs, rhs); }),
             std::make_pair(
                 "%", [](double lhs, double rhs) { return fmod(lhs, rhs); }),
             std::make_pair("+",
                            [](double lhs, double rhs) { return lhs + rhs; }),
             std::make_pair("-",
                            [](double lhs, double rhs) { return lhs - rhs; }),
             std::make_pair("*",
                            [](double lhs, double rhs) { return lhs * rhs; }),
             std::make_pair(
                 "/", [](double lhs, double rhs) { return lhs / rhs; })}){};

  typedef Tokenizer::TokenType TokenType;

  double Calculate(const list<std::string>& expr, double x = 0);

  vector<double> GenerateSet(double l, double r, int fin) const;

 private:
  // not static const due a little bit crutchy implementation of map
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
