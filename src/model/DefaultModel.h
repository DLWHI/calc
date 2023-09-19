#ifndef SRC_MODEL_DEFAULT_MODEL_H_
#define SRC_MODEL_DEFAULT_MODEL_H_

#include "calculator.h"
#include "icalculationmodel.h"
#include "tokenizer.h"
#include "translator.h"

namespace s21 {
class DefaultModel final : public ICalculationModel {
 public:
  typedef ICalculationModel BaseModel;
  typedef typename ICalculationModel::set_type set_type;

  static constexpr std::size_t kExprMaxSize = 255;
  static constexpr std::size_t kRangeFinesse = 1000;

  double Calculate(double x = 0) override {
    return machine_.Calculate(rpn_, x);
  };

  set_type Plot(double x_left, double x_right) override {
    vector<double> x_set = machine_.GenerateSet(x_left, x_right, kRangeFinesse);
    vector<double> y_set;
    y_set.reserve(kRangeFinesse);
    for (double x : x_set) y_set.push_back(machine_.Calculate(rpn_, x));
    return std::make_pair(x_set, y_set);
  }

  void setExpression(const std::string_view& expression) override {
    if (!expression.size()) throw BadExpression("Empty expression");
    if (expression.size() > kExprMaxSize)
      throw BadExpression("Expression is too long");
    if (expression.compare(expr_.c_str())) {
      rpn_ = to_polish_.Translate(fixer_.Tokenize(expression));
      expr_ = std::string(expression.begin(), expression.end());
    }
  }

  bool ExressionChanged() noexcept override {
    return fixer_.ExpressionChanged();
  }

 private:
  std::string expr_;
  list<std::string> rpn_;
  Tokenizer fixer_;
  Translator to_polish_;
  Calculator machine_;
};
}  // namespace s21

#endif  // SRC_MODEL_DEFAULT_MODEL_H_
