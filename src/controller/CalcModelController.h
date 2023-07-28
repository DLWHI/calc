#ifndef SRC_CONTROLLER_CALC_MODEL_CONTROLLER_H_
#define SRC_CONTROLLER_CALC_MODEL_CONTROLLER_H_

#include <cmath>
#include <type_traits>
#include "../model/ICalculationModel.h"
#include "../view/ICalculatorView.h"

namespace s21 {
class CalcModelController {
  public:
    static_assert(std::is_same<ICalculationModel::set_type,
                               ICalculatorView::set_type>::value, "");

    typedef ICalculationModel::set_type set_type;

    CalcModelController(ICalculationModel* model, ICalculatorView* view) : model_(model), view_(view) {
      view_->SubscribeExprChanged(std::bind(&CalcModelController::ExprChangedEvent, this, std::placeholders::_1));
      view_->SubscribeExprEval(std::bind(&CalcModelController::EvaluationEvent, this, std::placeholders::_1));
      view_->SubscribePlotEval(std::bind(&CalcModelController::PlotEvent, this, std::placeholders::_1, std::placeholders::_2));
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
        if (model_->ExressionChanged())
          view_->SendError("Note: An attempt was made to fix expression");
      } catch (std::exception& err) {
        view_->SendError("Error: " + std::string(err.what()));
      } catch (...) {
        view_->SendError("Unkown error occured");
      }
      return eval_result;
    }

    set_type PlotEvent(double left, double right) {
      set_type eval_result;
      try {
        if (expr_changed_) {
          expr_ = view_->GetExpr();
          model_->setExpression(expr_);
          expr_changed_ = false;
        }
        eval_result = model_->Plot(left, right);
        if (model_->ExressionChanged())
          view_->SendError("Note: An attempt was made to fix expression");
      } catch (std::exception& err) {
        view_->SendError("Error: " + std::string(err.what()));
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
