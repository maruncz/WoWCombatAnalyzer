#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CombatLog/exceptions.h"
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushFileBrowse_clicked()
{
    auto filename =
        QFileDialog::getOpenFileName(this, "Combat log", ".", "*.txt");

    ui->lineFileName->setText(filename);
}

void MainWindow::on_pushFileLoad_clicked()
{
    try
    {
        log = CombatLog::fromFile(ui->lineFileName->text());
    }
    catch (const CombatLogParserException &e)
    {

        auto msg = QString{"Exception:\nfile: %1 (%2:%3) '%4': %5"}
                       .arg(e.getPlace().file_name())
                       .arg(e.getPlace().line())
                       .arg(e.getPlace().column())
                       .arg(e.getPlace().function_name(), e.what());

        std::cerr << msg.toStdString() << '\n';

        QMessageBox::critical(this, "Error loading file", msg);
    }
    auto *widget =
        qobject_cast<DamagePerSecond *>(ui->tabWidget->currentWidget());

    if (!widget)
    {
        throw std::runtime_error("casting error");
    }

    widget->setLog(&log);
}
