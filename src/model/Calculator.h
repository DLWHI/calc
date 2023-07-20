#ifndef SRC_MODEL_CALCULATOR_H_
#define SRC_MODEL_CALCULATOR_H_
#include <stdexcept>

#include "ICalculationModel.h"

namespace s21 {
class Calculator: public ICalculationModel  {
  public:
    double Calculate() const;

    vector<double> CalculateSet(const vector<double>& points) const;
    vector<double> CalculateSet(double l, double r) const;

    vector<double> GenerateSet(double l, double r) const;

    void setExpression(const list<std::string>& expr) { expression = expr;};
  protected:
    void Operate(const std::string& operation, stack<double>& calc_stack) const;

    list<std::string> expression;
};
}  // namespace s21

#endif  // SRC_MODEL_CALCULATOR_H_
