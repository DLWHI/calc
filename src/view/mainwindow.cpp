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

    ui->plot->setBackground(QColor(0x1f, 0x1f, 0x1f, 0xff));

    ui->plot->xAxis->setBasePen(QColor(0xff, 0xff, 0xff, 0xff));
    ui->plot->xAxis->setTickLabelColor(QColor(0xff, 0xff, 0xff, 0xff));
    ui->plot->xAxis->setLabelColor(QColor(0xff, 0xff, 0xff, 0xff));

    ui->plot->yAxis->setBasePen(QColor(0xff, 0xff, 0xff, 0xff));
    ui->plot->yAxis->setTickLabelColor(QColor(0xff, 0xff, 0xff, 0xff));
    ui->plot->yAxis->setLabelColor(QColor(0xff, 0xff, 0xff, 0xff));
}

MainWindow::~MainWindow()
{
    delete ui;
}

