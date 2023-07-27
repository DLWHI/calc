#ifndef SRC_VIEW_I_CALCULATOR_VIEW_H_
#define SRC_VIEW_I_CALCULATOR_VIEW_H_

#include <string>
#include <functional>
#include "../containers/vector.h"

namespace s21 {
class ICalculatorView {
  public:
    typedef std::function<void(const std::string&)> ExprChangedDelegate;
    typedef std::function<double(double)> ExprEvalDelegate;
    typedef std::function<vector<double>(const vector<double>&)> PlotEvalDelegate;

    virtual ~ICalculatorView() = default;

    virtual void SubscribeExprChanged(const ExprChangedDelegate& delegate) = 0;

    virtual void SubscribeExprEval(const ExprEvalDelegate& delegate) = 0;

    virtual void SubscribePlotEval(const PlotEvalDelegate& delegate) = 0;

    virtual void SendError(const std::string& msg) = 0;

    virtual std::string GetExpr() = 0;
};
}  // namespace s21

#endif  // SRC_VIEW_I_CALCULATOR_VIEW_H_
