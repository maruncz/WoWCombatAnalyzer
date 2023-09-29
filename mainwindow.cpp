#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CombatLog/exceptions.h"
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#if 0
    //auto ret = LogLine::fromRawData("9/23 09:11:59.611  SPELL_AURA_REMOVED,0x000000000002FCA4,\"Marund\",0x511,0x000000000002FCA4,\"Marund\",0x511,57399,\"Well Fed\",0x1,BUFF");
    //auto ret = LogLine::fromRawData("9/22 17:35:04.139  SPELL_CAST_SUCCESS,0x0000000000026428,\"Blackintos\",0x512,0x0000000000000000,nil,0x80000000,49222,\"Bone Shield\",0x8");

    try {

        FilterObject filter;
        filter.setSourceName("Marund");



        //filter.addSubevent(SubEvent::SPELL_AURA_APPLIED);
        //filter.addSubevent(SubEvent::SPELL_AURA_APPLIED_DOSE);
        //filter.addSubevent(SubEvent::SPELL_AURA_REMOVED);
        //filter.addSubevent(SubEvent::SPELL_AURA_REFRESH);
        //filter.addSubevent(SubEvent::SPELL_DAMAGE);
        //filter.addSubevent(SubEvent::SPELL_ENERGIZE);
        //filter.addSubevent(SubEvent::SPELL_PERIODIC_DAMAGE);
        //filter.addSubevent(SubEvent::SWING_DAMAGE);
        //filter.addSubevent(SubEvent::DAMAGE_SHIELD);
        filter.addSubevent(SubEvent::SPELL_PERIODIC_HEAL);
        //filter.addSubevent(SubEvent::SPELL_CAST_START);
        //filter.addSubevent(SubEvent::SPELL_PERIODIC_ENERGIZE);
        filter.addSubevent(SubEvent::SPELL_HEAL);
        //filter.addSubevent(SubEvent::SWING_MISSED);
        //filter.addSubevent(SubEvent::PARTY_KILL);
        //filter.addSubevent(SubEvent::UNIT_DIED);
        //filter.addSubevent(SubEvent::SPELL_CAST_FAILED);
        //filter.addSubevent(SubEvent::ENCHANT_APPLIED);
        //filter.addSubevent(SubEvent::SPELL_MISSED);
        //filter.addSubevent(SubEvent::SPELL_INTERRUPT);
        //filter.addSubevent(SubEvent::SPELL_PERIODIC_MISSED);
        //filter.addSubevent(SubEvent::SPELL_DISPEL);
        //filter.addSubevent(SubEvent::SPELL_SUMMON);
        //filter.addSubevent(SubEvent::DAMAGE_SHIELD_MISSED);
        //filter.addSubevent(SubEvent::SPELL_PERIODIC_LEECH);


        log = CombatLog::fromFile("C:/Users/martin/Documents/wow-logy/WoWCombatLog-cos.txt",filter);

        std::cout << "parsed" << std::endl;



        //ui->tableView->setModel(&model);


        //ui->widget->setChart(&chart);

        chart.addSeries(&line);

        auto offset = log.getLines().front().getTimestamp().toMSecsSinceEpoch();
        auto end = log.getLines().back().getTimestamp().toMSecsSinceEpoch() - offset;

        axisX.setRange(0, end);

        axisY.setRange(0,10000);

        chart.addAxis(&axisX, Qt::AlignBottom);
        chart.addAxis(&axisY, Qt::AlignLeft);
        line.attachAxis(&axisX);
        line.attachAxis(&axisY);

        SubSampler sampler{5000,&line};

        for(const auto  &e : log.getLines())
        {
            sampler.addValue(e.getTimestamp().toMSecsSinceEpoch() - offset, std::visit([](auto &&arg) -> qreal {
                                 using T = std::decay_t<decltype(arg)>;
                                 if constexpr (std::is_base_of_v<detail::suffix::Heal,T>)
                                 {
                                     return arg.amount;
                                 }
                                 else
                                 {
                                     throw std::runtime_error("unhandled case");
                                 }
                             },e.getSubEventValue()));
        }

        sampler.finalize();


        {
            auto [min, max] = sampler.getMinmax();
            axisY.setRange(min,max);
        }



    } catch (const CombatLogParserException &e)
    {
        std::cerr << "file: "
                  << e.getPlace().file_name() << '('
                  << e.getPlace().line() << ':'
                  << e.getPlace().column() << ") `"
                  << e.getPlace().function_name() << "`: "
                  << e.what() << '\n';
    }
#endif


}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_pushFileBrowse_clicked()
{
    auto filename = QFileDialog::getOpenFileName(this,"Combat log",".","*.txt");

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
                       .arg(e.getPlace().line()).arg(e.getPlace().column())
            .arg(e.getPlace().function_name(),e.what());

        std::cerr << msg.toStdString() << '\n';

        QMessageBox::critical(this,"Error loading file",msg);
    }
    auto * widget = qobject_cast<DamagePerSecond*>(ui->tabWidget->currentWidget());

    if(!widget)
    {
        throw std::runtime_error("casting error");
    }

    widget->setLog(&log);

}

