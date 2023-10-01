#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CombatLog/CombatLog.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:

    void on_pushFileBrowse_clicked();

    void on_pushFileLoad_clicked();

private:
    Ui::MainWindow *ui;
    CombatLog log;
};
#endif // MAINWINDOW_H
