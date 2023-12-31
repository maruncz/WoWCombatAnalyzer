#ifndef DAMAGEPERSECOND_H
#define DAMAGEPERSECOND_H

#include <QWidget>
#include <QLineSeries>
#include <QChartView>
#include <QValueAxis>
#include <QList>
#include <utility>
#include "../DataModels/ObjectListModel.h"

namespace Ui {
class DamagePerSecond;
}

class CombatLog;


class DamagePerSecond : public QWidget
{
    Q_OBJECT

public:
    explicit DamagePerSecond(QWidget *parent = nullptr);
    ~DamagePerSecond();

    void setLog(CombatLog *newLog);

public slots:

    void updateChart();

private slots:
    void on_pushRefresh_clicked();

private:

    void adjustXRange(qreal min, qreal max);
    void adjustYRange(qreal min, qreal max);

    Ui::DamagePerSecond *ui;

    ObjectListModel sourceModel;
    ObjectListModel targetModel;

    QStringList sourceNamesList;
    QStringList targetNamesList;


    CombatLog *originalLog;
    QChart chart;
    QValueAxis axisX;
    QValueAxis axisY;
};

#endif // DAMAGEPERSECOND_H
