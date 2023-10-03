#ifndef SOURCESELECTOR_H
#define SOURCESELECTOR_H

#include <QWidget>

namespace Ui
{
class SourceSelector;
} // namespace Ui

class SourceSelector : public QWidget
{
    Q_OBJECT

public:
    explicit SourceSelector(QWidget *parent = nullptr);
    ~SourceSelector() override;

private:
    Ui::SourceSelector *ui;
};

#endif // SOURCESELECTOR_H
