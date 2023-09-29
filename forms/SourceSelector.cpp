#include "SourceSelector.h"
#include "ui_SourceSelector.h"

SourceSelector::SourceSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SourceSelector)
{
    ui->setupUi(this);
}

SourceSelector::~SourceSelector()
{
    delete ui;
}
