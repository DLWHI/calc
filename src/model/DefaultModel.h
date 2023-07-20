#ifndef SRC_MODEL_DEFAULT_MODEL_H_
#define SRC_MODEL_DEFAULT_MODEL_H_
#include <stdexcept>

#include "ICalculationModel.h"
#include "Tokenizer.h"
#include "Translator.h"
#include "Calculator.h"

namespace s21 {
class DefaultModel : public ICalculationModel {
  public:
    double Calculate(double x = 0) {
      return machine.Calculate(expr_, x);
    };
    vector<double> CalculateSet(const vector<double>& points) {
      vector<double> expr_of_x(points.size());
      for (auto x: points) {
        expr_of_x.push_back(machine.Calculate(expr_, x));
      }
      return expr_of_x;
    };

    vector<double> CalculateSet(double l, double r) {
      // dynamic finnese
    };
    
    vector<double> GenerateSet(double l, double r, int fin) const {
      if (l > r)
       throw std::domain_error("Invalid set boundaries");
      vector<double> points(fin);
      double delta = (r - l)/fin;
      for (; l != r; l += delta)
        points.push_back(l);
      return points;
    };

    void setExpression(const std::string_view& expression) { 
      expr_ = to_polish.Translate(fixer.Tokenize(expression));
    }
  private:

    list<std::string> expr_;
    Tokenizer fixer;
    Translator to_polish;
    Calculator machine;
};
}  // namespace s21

#endif  // SRC_MODEL_DEFAULT_MODEL_H_
