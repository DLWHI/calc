#ifndef SRC_CONTROLLER_CALC_MODEL_CONTROLLER_H_
#define SRC_CONTROLLER_CALC_MODEL_CONTROLLER_H_

#include <cmath>
#include "../model/ICalculationModel.h"
#include "../view/ICalculatorView.h"

namespace s21 {
class CalcModelController {
  public:
    CalcModelController(ICalculationModel* model, ICalculatorView* view) : model_(model), view_(view) {
      view_->SubscribeExprChanged(std::bind(&CalcModelController::ExprChangedEvent, this, std::placeholders::_1));
      view_->SubscribeExprEval(std::bind(&CalcModelController::EvaluationEvent, this, std::placeholders::_1));
    };

  private:
    void ExprChangedEvent(const std::string& new_expr) {
      if (new_expr == expr_)
        expr_changed_ = false;
      else
        expr_changed_ = true;
    }

    double EvaluationEvent(double x) {
      double eval_result = NAN;
      try {
        if (expr_changed_) {
          expr_ = view_->GetExpr();
          model_->setExpression(expr_);
          expr_changed_ = false;
        }
        eval_result = model_->Calculate(x);
      } catch (std::logic_error& err) {
        view_->SendError(err.what());
      } catch (...) {
        view_->SendError("Unkown error occured");
      }
      return eval_result;
    }

    bool expr_changed_;
    std::string expr_;
    std::string err_msg_;
    ICalculationModel* model_;
    ICalculatorView* view_;
};
}  // namespace s21

#endif  // SRC_CONTROLLER_CALC_MODEL_CONTROLLER_H_
