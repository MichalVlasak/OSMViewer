#include "GpxInfosWidget.h"
#include "ui_GpxInfosWidget.h"
#include "GpxInfoFileWidget.h"

#include <QTabWidget>

GpxInfosWidget::GpxInfosWidget(GpxManager * gpxManager, GpxLayer * gpxLayer, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GpxInfosWidget),
    _gpxManager(gpxManager),
    _gpxLayer(gpxLayer)
{
    _ui->setupUi(this);

    _ui->tabWidget->clear();
}

GpxInfosWidget::~GpxInfosWidget()
{
    delete _ui;
}

void GpxInfosWidget::changeSelectedGps(const GpxManager::GpxIdVector &ids)
{
    _showedGpxIds = ids;

    for(int i = 0; i < _ui->tabWidget->count(); i++)
    {
        QWidget * widget = _ui->tabWidget->widget(i);

        if(widget != nullptr)
        {
            GpxInfoFileWidget * gpxWidget = dynamic_cast<GpxInfoFileWidget*>(widget);

            if(gpxWidget != nullptr)
            {
                gpxWidget->clearSelectedPoint();
            }
        }
    }

    _ui->tabWidget->clear();

    if(_showedGpxIds.size() > 0)
    {
        if(_gpxManager != nullptr)
        {
            const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

            for(int id : _showedGpxIds)
            {
                for(const GpxManager::GpxItem & item : gpxVector)
                {
                    if(item.fileId == id)
                    {
                        QFileInfo fileInfo(item.filePath);

                        GpxInfoFileWidget * gpxWidget = new GpxInfoFileWidget(_gpxManager, _gpxLayer, id, this);

                        QObject::connect(gpxWidget, SIGNAL(centerMap(QPoint)), SIGNAL(centerMap(QPoint)));

                        _ui->tabWidget->addTab(gpxWidget, fileInfo.fileName());
                    }
                }
            }
        }
    }
}

void GpxInfosWidget::deleteAll()
{
    for(int i = 0; i < _ui->tabWidget->count(); i++)
    {
        QWidget * widget = _ui->tabWidget->widget(i);

        if(widget != nullptr)
        {
            GpxInfoFileWidget * gpxWidget = dynamic_cast<GpxInfoFileWidget*>(widget);

            if(gpxWidget != nullptr)
            {
                QObject::disconnect(gpxWidget, SIGNAL(centerMap(QPoint)), this, SIGNAL(centerMap(QPoint)));
            }
        }
    }

    _ui->tabWidget->clear();
}

void GpxInfosWidget::deleteGpx(int id)
{

}
