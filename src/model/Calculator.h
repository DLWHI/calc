#ifndef SRC_MODEL_CALCULATOR_H_
#define SRC_MODEL_CALCULATOR_H_
#include <stdexcept>

#include "ICalculationModel.h"

namespace s21 {
class Calculator: public ICalculationModel  {
  public:
    double calculate();
    vector<double> calculate_set(const vector<double>& points);
    
    vector<double> generate_set(double l, double r);

    void setExpression(const vector<std::string>& expr);
  protected:
    std::string expression;
};
}

#endif  // SRC_MODEL_CALCULATOR_H_
