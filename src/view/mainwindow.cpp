#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFontDatabase>
#include <QFile>
#include <QTextStream>
#include <iostream>

const s21::set<QString> MainWindow::banned_buttons = {
    QString("button_ac"),
    QString("button_del"),
    QString("button_eq")
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetRestrictions();
    LoadStyle();
    ConnectEvents();
}

std::string MainWindow::GetExpr() {
    return ui->edit_input->text().toStdString();
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

void MainWindow::ConnectEvents() {
    QPushButton* target_button;
    for (auto button: ui->Buttons->children()) {
        if (!banned_buttons.contains(button->objectName())) {
            target_button = dynamic_cast<QPushButton*>(button);
            connect(target_button, &QPushButton::clicked, this, &MainWindow::InputButtonPressed);
        }
    }
    for (auto func: ui->Functions->children()) {
        target_button = dynamic_cast<QPushButton*>(func);
        connect(target_button, &QPushButton::clicked, this, &MainWindow::InputButtonPressed);
    }
    for (auto tr_func: ui->TrigFunctions->children()) {
        target_button = dynamic_cast<QPushButton*>(tr_func);
        connect(target_button, &QPushButton::clicked, this, &MainWindow::InputButtonPressed);
    }
    connect(ui->button_ac, &QPushButton::clicked, this, &MainWindow::ClearAll);
    connect(ui->button_del, &QPushButton::clicked, this, &MainWindow::DelSymbol);
    connect(ui->button_eq, &QPushButton::clicked, this, &MainWindow::Eval);

    connect(ui->edit_input, &QLineEdit::textChanged, this, &MainWindow::OnExprChanged);
}

void MainWindow::InputButtonPressed() {
    QPushButton* sender_button = dynamic_cast<QPushButton*>(sender());
    ui->edit_input->insert(sender_button->text());
}

void MainWindow::DelSymbol() {
    ui->edit_input->backspace();
}
    
void MainWindow::ClearAll() {
    ui->edit_input->clear();
    ui->input_x->clear();
    ui->input_xl->clear();
    ui->input_xr->clear();
    ui->input_yl->clear();
    ui->input_yr->clear();
}

void MainWindow::SendError(const std::string& msg) {
    std::cout << msg << std::endl;
}

void MainWindow::OnExprChanged(const QString &text) {
    on_expr_changed_(text.toStdString());
}

void MainWindow::Eval() {
    bool succ;
    double arg = ui->input_x->text().toDouble(&succ);
    if (succ)
        ui->label_output->setText(QString::number(on_eval_(arg)));
    else
        SendError("Invalid x argument");
}

void MainWindow::Plot() {
    
}

void MainWindow::SubscribeExprChanged(const ExprChangedDelegate& delegate) {
    on_expr_changed_ = delegate;
}

void MainWindow::SubscribeExprEval(const ExprEvalDelegate& delegate) {
    on_eval_ = delegate;
}

void MainWindow::SubscribePlotEval(const PlotEvalDelegate& delegate) {
    on_plot_ = delegate;
}

MainWindow::~MainWindow()
{
    delete ui;
}
