#include "DamagePerSecond.h"
#include "ui_DamagePerSecond.h"
#include "../CombatLog/CombatLog.h"
#include "../filters/SubSampler.h"
#include <limits>
#include <cmath>
#include <QDebug>

DamagePerSecond::DamagePerSecond(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DamagePerSecond)
{
    ui->setupUi(this);
    ui->listSource->setModel(&sourceModel);
    ui->listTarget->setModel(&targetModel);
    ui->chartView->setChart(&chart);
    chart.addAxis(&axisX, Qt::AlignBottom);
    chart.addAxis(&axisY, Qt::AlignLeft);


}

DamagePerSecond::~DamagePerSecond()
{
    delete ui;
}

void DamagePerSecond::updateChart()
{
    if(!originalLog)
    {
        throw std::runtime_error("log ptr null");
    }

    chart.removeAllSeries();

    axisX.setRange(0,0);
    axisY.setRange(0,0);

    auto selectedSources = ui->listSource->selectionModel()->selectedRows();

    auto timeOffset = originalLog->getLines().front().getTimestamp().toMSecsSinceEpoch();

    for(const auto &e : selectedSources)
    {
        auto sourceName = sourceNamesList.at(e.row());

        auto filter = [sourceName](const LogLine &l)
        {
            if(l.getSourceObject().name != sourceName)
            {
                return false;
            }

            if((l.getSubeventType() == SubEvent::SPELL_DAMAGE)
                || (l.getSubeventType() == SubEvent::SPELL_PERIODIC_DAMAGE)
                || (l.getSubeventType() == SubEvent::SWING_DAMAGE))
            {
                return true;
            }
            return false;
        };

        auto temporaryLog = originalLog->filter(filter);



        if(temporaryLog.getLines().empty())
        {
            continue;
        }

        auto lineOffset = temporaryLog.getLines().front().getTimestamp().toMSecsSinceEpoch() - timeOffset;

        auto line = new QLineSeries(this);
        SubSampler sampler{ui->doubleSpinBoxFilter->value()*1000, static_cast<qreal>(lineOffset)};

        for(const auto  &e : temporaryLog.getLines())
        {
#if 0
            qDebug() << "in:" << e.getTimestamp().toMSecsSinceEpoch() - timeOffset << ',' <<                              std::visit([](auto &&arg) -> qreal {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_base_of_v<detail::suffix::Damage,T>)
                {
                    return arg.amount;
                }
                else
                {
                    throw std::runtime_error("unhandled case");
                }
            },e.getSubEventValue());
#endif

            auto samples = sampler.addValue(e.getTimestamp().toMSecsSinceEpoch() - timeOffset,
                             std::visit([](auto &&arg) -> qreal {
                                 using T = std::decay_t<decltype(arg)>;
                                 if constexpr (std::is_base_of_v<detail::suffix::Damage,T>)
                                 {
                                     return arg.amount;
                                 }
                                 else
                                 {
                                     throw std::runtime_error("unhandled case");
                                 }
                             },e.getSubEventValue()));
            if(samples.has_value())
            {
                for(const auto &sample : samples.value())
                {
                    line->append(sample.first, sample.second);
                }
                //qDebug() << "out:" << sample->first << ',' << sample->second;
            }
        }

        {
            auto sample = sampler.finalize();
            line->append(sample.first, sample.second);
            //qDebug() << "out:" << sample.first << ',' << sample.second;
            adjustYRange(sampler.getMinmaxY().first,sampler.getMinmaxY().second);
            adjustXRange(sampler.getMinmaxX().first,sampler.getMinmaxX().second);
        }



        chart.addSeries(line);
        line->attachAxis(&axisX);
        line->attachAxis(&axisY);
        line->setName(sourceName);

    }




}

void DamagePerSecond::adjustXRange(qreal min, qreal max)
{
    auto oldMin = axisX.min();
    if(std::abs(oldMin) < 1)
    {
        axisX.setMin(min);
    }
    else if(oldMin > min)
    {
        axisX.setMin(min);
    }

    auto oldMax = axisX.max();
    if(std::abs(oldMax) < 1)
    {
        axisX.setMax(max);
    }
    else if(oldMax < max)
    {
        axisX.setMax(max);
    }
}

void DamagePerSecond::adjustYRange(qreal min, qreal max)
{
    auto oldMin = axisY.min();
    if(oldMin > min)
    {
        axisY.setMin(min);
    }

    auto oldMax = axisY.max();
    if(oldMax < max)
    {
        axisY.setMax(max);
    }
}


void DamagePerSecond::setLog(CombatLog *newLog)
{
    originalLog = newLog;
    ui->listSource->selectionModel()->clearSelection();

    auto filterSources = [](const LogLine &l)
    {
        return (l.getSourceObject().flags.value & UnitFlags::Control::player) != 0;
    };
    sourceNamesList = originalLog->filter(filterSources).getSourceNames();
    sourceModel.setList(&sourceNamesList);

    auto filterTargets = [](const LogLine &l)
    {
        return (l.getDestObject().flags.value & UnitFlags::Control::npc) != 0;
    };
    targetNamesList = originalLog->filter(filterTargets).getTargetNames();
    targetModel.setList(&targetNamesList);

}

void DamagePerSecond::on_pushRefresh_clicked()
{
    updateChart();
}

