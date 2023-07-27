#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include "ICalculatorView.h"
#include "../containers/s21_set.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public s21::ICalculatorView 
{
    Q_OBJECT

public:
    typedef s21::ICalculatorView BaseView;
    typedef BaseView::ExprChangedDelegate ExprChangedDelegate;
    typedef BaseView::ExprEvalDelegate ExprEvalDelegate;
    typedef BaseView::PlotEvalDelegate PlotEvalDelegate;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SubscribeExprChanged(const ExprChangedDelegate& delegate) override;

    void SubscribeExprEval(const ExprEvalDelegate& delegate) override;

    void SubscribePlotEval(const PlotEvalDelegate& delegate) override;

    void SendError(const std::string& msg) override;

    std::string GetExpr() override;
private:
    void SetRestrictions();
    void LoadStyle();
    void ConnectEvents();

    void OnExprChanged(const QString &text);
    void InputButtonPressed();
    void DelSymbol();
    void ClearAll();
    void Eval();
    void Plot();

    static const s21::set<QString> banned_buttons;

    Ui::MainWindow *ui;

    ExprChangedDelegate on_expr_changed_;
    ExprEvalDelegate on_eval_;
    PlotEvalDelegate on_plot_;
};
#endif // MAINWINDOW_H
