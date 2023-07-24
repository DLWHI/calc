#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LoadFont();
}

void MainWindow::LoadFont() {
    int id = QFontDatabase::addApplicationFont(":/fonts/Tektur.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont kf(family);
    kf.setPointSize(22);
    setFont(kf);
}

MainWindow::~MainWindow()
{
    delete ui;
}

