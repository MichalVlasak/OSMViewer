#include "GpxInfoFileWidget.h"
#include "ui_GpxInfoFileWidget.h"

GpxInfoFileWidget::GpxInfoFileWidget(GpxManager * gpxManager, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GpxInfoFileWidget),
    _gpxManager(gpxManager)
{
    _ui->setupUi(this);

    _tableMode = new GpxInfoFileModel(this);

    _ui->tableView->setModel(_tableMode);
}

GpxInfoFileWidget::~GpxInfoFileWidget()
{
    delete _ui;
}

void GpxInfoFileWidget::changeSelectedGps(const GpxManager::GpxIdVector &ids)
{
    for(int id : ids)
    {
    }
}
