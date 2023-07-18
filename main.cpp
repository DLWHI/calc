#include "src/view/mainwindow.h"

#include <QApplication>
#include <iostream>

#include "src/model/Translator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
