#ifndef SRC_CONTROLLER_CALC_MODEL_CONTROLLER_H_
#define SRC_CONTROLLER_CALC_MODEL_CONTROLLER_H_

#include <cmath>

#include "../model/icalculationmodel.h"
#include "../view/icalculatorview.h"

namespace s21 {
class CalcModelController {
 public:
  static_assert(std::is_same<ICalculationModel::set_type,
                             ICalculatorView::set_type>::value,
                "");

  typedef ICalculationModel::set_type set_type;

  CalcModelController(ICalculationModel* model, ICalculatorView* view)
      : model_(model), view_(view) {
    view_->SubscribeExprEval(std::bind(&CalcModelController::EvaluationEvent,
                                       this, std::placeholders::_1));
    view_->SubscribePlotEval(std::bind(&CalcModelController::PlotEvent, this,
                                       std::placeholders::_1,
                                       std::placeholders::_2));
  };

 private:
  double EvaluationEvent(double x) {
    double eval_result = NAN;
    try {
      model_->setExpression(view_->GetExpr());
      eval_result = model_->Calculate(x);
      if (model_->ExressionChanged())
        view_->SendError("Note: An attempt was made to fix expression");
    } catch (BadExpression& err) {
      view_->SendError("Error: " + std::string(err.what()));
    }
    return eval_result;
  }

  set_type PlotEvent(double left, double right) {
    set_type eval_result;
    try {
      model_->setExpression(view_->GetExpr());
      eval_result = model_->Plot(left, right);
      if (model_->ExressionChanged())
        view_->SendError("Note: An attempt was made to fix expression");
    } catch (BadExpression& err) {
      view_->SendError("Error: " + std::string(err.what()));
    }
    return eval_result;
  }

  ICalculationModel* model_;
  ICalculatorView* view_;
};
}  // namespace s21

#endif  // SRC_CONTROLLER_CALC_MODEL_CONTROLLER_H_
