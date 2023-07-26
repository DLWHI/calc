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
    SetRestrictions();
    LoadStyle();
}

void MainWindow::SetRestrictions() {
    ui->input_x->setValidator(new QDoubleValidator());
    ui->input_xl->setValidator(new QDoubleValidator());
    ui->input_xr->setValidator(new QDoubleValidator());
    ui->input_yl->setValidator(new QDoubleValidator());
    ui->input_yr->setValidator(new QDoubleValidator());
}

void MainWindow::LoadStyle() {
    QFontDatabase::addApplicationFont(":/fonts/Tektur.ttf");
    
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

