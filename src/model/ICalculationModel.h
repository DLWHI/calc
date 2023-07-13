#ifndef SRC_MODEL_I_CALCULATION_MODEL_H_
#define SRC_MODEL_I_CALCULATION_MODEL_H_
#include "../containers/vector.h"

namespace s21 {
class ICalculationModel {
  public:
    virtual double calculate() = 0;
    virtual vector<double> calculate_set(const vector<double>& points) = 0;
    
    virtual vector<double> generate_set(double l, double r) = 0;

    virtual void setExpression(const vector<std::string>& expr) = 0;
};
}

#endif  // SRC_MODEL_I_CALCULATION_MODEL_H_