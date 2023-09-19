#include "src/model/defaultmodel.h"
#include "src/view/mainwindow.h"
#include "src/controller/calcmodelcontroller.h"

#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    s21::DefaultModel model;
    s21::CalcModelController troller(&model, &w);
    w.show();
    return a.exec();
}
