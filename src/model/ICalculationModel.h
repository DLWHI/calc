#ifndef SRC_MODEL_I_CALCULATION_MODEL_H_
#define SRC_MODEL_I_CALCULATION_MODEL_H_
#include <string_view>
#include "../containers/vector.h"

namespace s21 {
class ICalculationModel {
  public:
    virtual double Calculate(double x = 0) = 0;
    virtual vector<double> CalculateSet(const vector<double>& points) = 0;

    virtual vector<double> CalculateSet(double l, double r) = 0;
    
    virtual vector<double> GenerateSet(double l, double r, int fin) const = 0;

    virtual void setExpression(const std::string_view& expr) = 0;
};
}  // namespace s21

#endif  // SRC_MODEL_I_CALCULATION_MODEL_H_
