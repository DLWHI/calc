#ifndef SRC_MODEL_I_CALCULATION_MODEL_H_
#define SRC_MODEL_I_CALCULATION_MODEL_H_
#include <string_view>
#include "../containers/vector.h"

namespace s21 {
class ICalculationModel {
  public:
    virtual double Calculate(double x = 0) = 0;

    virtual std::pair<vector<double>, vector<double>> Plot(double x_left, double x_right) = 0;

    virtual void setExpression(const std::string_view& expr) = 0;
};
}  // namespace s21

#endif  // SRC_MODEL_I_CALCULATION_MODEL_H_
