#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CombatLog/CombatLog.h"
#include "CombatLog/exceptions.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //auto ret = LogLine::fromRawData("9/23 09:11:59.611  SPELL_AURA_REMOVED,0x000000000002FCA4,\"Marund\",0x511,0x000000000002FCA4,\"Marund\",0x511,57399,\"Well Fed\",0x1,BUFF");
    //auto ret = LogLine::fromRawData("9/22 17:35:04.139  SPELL_CAST_SUCCESS,0x0000000000026428,\"Blackintos\",0x512,0x0000000000000000,nil,0x80000000,49222,\"Bone Shield\",0x8");

    try {

        auto log = CombatLog::fromFile("C:/Users/martin/Documents/wow-logy/WoWCombatLog-cos.txt");
    } catch (const CombatLogParserException &e)
    {
        std::cerr << "file: "
                  << e.getPlace().file_name() << '('
                              << e.getPlace().line() << ':'
                              << e.getPlace().column() << ") `"
                              << e.getPlace().function_name() << "`: "
                              << e.what() << '\n';
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

