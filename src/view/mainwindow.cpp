#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFontDatabase>
#include <QFile>
#include <QTextStream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    LoadFont();
    LoadStyleSheet();
}

void MainWindow::LoadFont() {
    QFontDatabase::addApplicationFont(":/fonts/Tektur.ttf");
}

void MainWindow::LoadStyleSheet() {
    QFile style_file(":/styles/style.ss");
    style_file.open(QIODevice::ReadOnly);

    QTextStream style_stream(&style_file);
    setStyleSheet(style_stream.readAll());
}

MainWindow::~MainWindow()
{
    delete ui;
}

