#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <s21_set.h>

#include <QLineEdit>
#include <QMainWindow>

#include "icalculatorview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public s21::ICalculatorView {
  Q_OBJECT

 public:
  typedef s21::ICalculatorView BaseView;
  typedef BaseView::ExprChangedDelegate ExprChangedDelegate;
  typedef BaseView::ExprEvalDelegate ExprEvalDelegate;
  typedef BaseView::PlotEvalDelegate PlotEvalDelegate;

  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void SubscribeExprEval(const ExprEvalDelegate& delegate) override;

  void SubscribePlotEval(const PlotEvalDelegate& delegate) override;

  void SendError(const std::string& msg) override;

  std::string GetExpr() override;

 private:
  void SetRestrictions();
  void LoadStyle();
  void ConnectEvents();

  void OnExprChanged(const QString& text);
  void InputButtonPressed();
  void DelSymbol();
  void ClearAll();
  void Eval();
  void Plot();

  std::string ReplaceCrutch(const std::string& text) const;

  static const s21::set<QString> banned_buttons;

  Ui::MainWindow* ui;

  ExprEvalDelegate on_eval_;
  PlotEvalDelegate on_plot_;
};
#endif  // MAINWINDOW_H
