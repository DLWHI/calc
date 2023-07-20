#ifndef SRC_MODEL_I_CALCULATION_MODEL_H_
#define SRC_MODEL_I_CALCULATION_MODEL_H_
#include "../containers/s21_list.h"
#include "../containers/vector.h"

namespace s21 {
class ICalculationModel {
  public:
    virtual double Calculate() const = 0;
    virtual vector<double> CalculateSet(const vector<double>& points) const = 0;

    virtual vector<double> CalculateSet(double l, double r) const = 0;
    
    virtual vector<double> GenerateSet(double l, double r, int fin) const = 0;

    virtual void setExpression(const list<std::string>& expr) = 0;
};
}  // namespace s21

#endif  // SRC_MODEL_I_CALCULATION_MODEL_H_