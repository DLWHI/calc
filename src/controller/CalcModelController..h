#ifndef SRC_CONTROLLER_CALC_MODEL_CONTROLLER_H_
#define SRC_CONTROLLER_CALC_MODEL_CONTROLLER_H_

#include "../model/DefaultModel.h"

namespace s21 {
class CalcModelController {
  public:
    CalcModelController(ICalculationModel* target_model) : model_(target_model) { };

    

  private:
    ICalculationModel* model_;
};
}  // namespace s21

#endif  // SRC_CONTROLLER_CALC_MODEL_CONTROLLER_H_
