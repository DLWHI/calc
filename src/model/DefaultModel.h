#ifndef SRC_MODEL_DEFAULT_MODEL_H_
#define SRC_MODEL_DEFAULT_MODEL_H_
#include <stdexcept>

#include "ICalculationModel.h"
#include "Tokenizer.h"
#include "Translator.h"
#include "Calculator.h"

namespace s21 {
class DefaultModel final : public ICalculationModel {
  public:
    typedef ICalculationModel BaseModel;
    typedef typename ICalculationModel::set_type set_type;

    static constexpr std::size_t kExprMaxSize = 255;
    static constexpr std::size_t kRangeFinesse = 1000;

    double Calculate(double x = 0) override {
      return machine_.Calculate(expr_, x);
    };

    set_type Plot(double x_left, double x_right) override {
      vector<double> x_set = machine_.GenerateSet(x_left, x_right, kRangeFinesse);
      vector<double> y_set;
      y_set.reserve(kRangeFinesse);
      for (double x: x_set)
        y_set.push_back(machine_.Calculate(expr_, x));
      return std::make_pair(x_set, y_set);
    }

    void setExpression(const std::string_view& expression) override {
      if (expression.size() > kExprMaxSize)
        throw std::invalid_argument("Expression is too long");
      expr_ = to_polish_.Translate(fixer_.Tokenize(expression));
    }

    bool ExressionChanged() noexcept override { return fixer_.ExpressionChanged();}
  private:

    list<std::string> expr_;
    Tokenizer fixer_;
    Translator to_polish_;
    Calculator machine_;
};
}  // namespace s21

#endif  // SRC_MODEL_DEFAULT_MODEL_H_
