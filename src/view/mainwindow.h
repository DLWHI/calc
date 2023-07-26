#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include "../containers/s21_set.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void SetRestrictions();
    void LoadStyle();
    void ConnectEvents();

    void InputButtonPressed();
    void DelSymbol();
    void ClearAll();
    void Eval();
    void Plot();

    void GetFocusedEdit(QWidget * old, QWidget * now);

    void keyPressEvent(QKeyEvent *event) override;

    static const s21::set<QString> banned_buttons;

    Ui::MainWindow *ui;
    QLineEdit* focused_;
};
#endif // MAINWINDOW_H
