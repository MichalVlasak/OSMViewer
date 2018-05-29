#include "GpxInfosWidget.h"
#include "ui_GpxInfosWidget.h"
#include "GpxInfoFileWidget.h"

#include <QTabWidget>

GpxInfosWidget::GpxInfosWidget(GpxManager * gpxManager, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GpxInfosWidget),
    _gpxManager(gpxManager)
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

                        _ui->tabWidget->addTab(new GpxInfoFileWidget(_gpxManager, id, this), fileInfo.fileName());
                    }
                }
            }
        }
    }
}

void GpxInfosWidget::deleteAll()
{
    _ui->tabWidget->clear();
}

void GpxInfosWidget::deleteGpx(int id)
{

}
