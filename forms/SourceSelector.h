#ifndef SOURCESELECTOR_H
#define SOURCESELECTOR_H

#include <QWidget>

namespace Ui {
class SourceSelector;
}

class SourceSelector : public QWidget
{
    Q_OBJECT

public:
    explicit SourceSelector(QWidget *parent = nullptr);
    ~SourceSelector();

private:
    Ui::SourceSelector *ui;


};

#endif // SOURCESELECTOR_H
