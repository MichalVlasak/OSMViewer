#include "GpxInfoFileWidget.h"
#include "ui_GpxInfoFileWidget.h"

GpxInfoFileWidget::GpxInfoFileWidget(GpxManager * gpxManager, GpxLayer * gpxLayer, int gpxId, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GpxInfoFileWidget),
    _gpxManager(gpxManager),
    _gpxId(gpxId),
    _gpxLayer(gpxLayer)
{
    _ui->setupUi(this);

    _tableModel = new GpxInfoFileModel(this);

    _ui->tableView->setModel(_tableModel);

    QObject::connect(_ui->maxCadention, SIGNAL(clicked(bool)), SLOT(selectMaxCadention()));
    QObject::connect(_ui->maxElevation, SIGNAL(clicked(bool)), SLOT(selectMaxElevation()));
    QObject::connect(_ui->maxHeartRate, SIGNAL(clicked(bool)), SLOT(selectMaxHeartRate()));
    QObject::connect(_ui->maxTemperature, SIGNAL(clicked(bool)), SLOT(selectMaxTemperature()));
    QObject::connect(_ui->clearSelection, SIGNAL(clicked(bool)), SLOT(clearSelectedPoint()));
    QObject::connect(_ui->center, SIGNAL(clicked(bool)), SLOT(centerMap()));
    QObject::connect(_ui->tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(selectionChanged(QItemSelection,QItemSelection)));
    QObject::connect(_gpxLayer, SIGNAL(changeSelectedPoint(int,size_t)), SLOT(changeSelectedPoint(int,size_t)));

    fillTable();

    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & item : gpxVector)
        {
            if(item.fileId == _gpxId)
            {
                if(item.biggestCadentionIdx == -1)
                {
                    _ui->maxCadention->setDisabled(true);
                }

                if(item.biggestElevetionIdx == -1)
                {
                    _ui->maxElevation->setDisabled(true);
                }

                if(item.biggestHeartRateIdx == -1)
                {
                    _ui->maxHeartRate->setDisabled(true);
                }

                if(item.biggestTemperatureIdx == -1)
                {
                    _ui->maxTemperature->setDisabled(true);
                }
            }
        }
    }
}

GpxInfoFileWidget::~GpxInfoFileWidget()
{
    clearSelectedPoint();

    delete _ui;
}

void GpxInfoFileWidget::fillTable()
{
    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & item : gpxVector)
        {
            if(item.fileId == _gpxId)
            {
                if(_tableModel != nullptr)
                {
                    _tableModel->setItemPoints(item.pointVector);
                }
            }
        }
    }
}

void GpxInfoFileWidget::selectMaxCadention()
{
    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & item : gpxVector)
        {
            if(item.fileId == _gpxId)
            {
                if(item.biggestCadentionIdx > -1 && size_t(item.biggestCadentionIdx) < item.pointVector.size())
                {
                    _ui->tableView->selectRow(item.biggestCadentionIdx);
                }
            }
        }
    }
}

void GpxInfoFileWidget::selectMaxElevation()
{
    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & item : gpxVector)
        {
            if(item.fileId == _gpxId)
            {
                if(item.biggestElevetionIdx > -1 && size_t(item.biggestElevetionIdx) < item.pointVector.size())
                {
                    _ui->tableView->selectRow(item.biggestElevetionIdx);
                }
            }
        }
    }
}

void GpxInfoFileWidget::selectMaxHeartRate()
{
    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & item : gpxVector)
        {
            if(item.fileId == _gpxId)
            {
                if(item.biggestHeartRateIdx > -1 && size_t(item.biggestHeartRateIdx) < item.pointVector.size())
                {
                    _ui->tableView->selectRow(item.biggestHeartRateIdx);
                }
            }
        }
    }
}

void GpxInfoFileWidget::selectMaxTemperature()
{
    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & item : gpxVector)
        {
            if(item.fileId == _gpxId)
            {
                if(item.biggestTemperatureIdx > -1 && size_t(item.biggestTemperatureIdx) < item.pointVector.size())
                {
                    _ui->tableView->selectRow(item.biggestTemperatureIdx);
                }
            }
        }
    }
}

void GpxInfoFileWidget::selectionChanged(QItemSelection selected, QItemSelection deselected)
{
    QModelIndexList indexes = _ui->tableView->selectionModel()->selectedIndexes();
    int prevColumn = -1;

    for(const QModelIndex & index : indexes)
    {
        if(index.isValid() == true && index.column() != prevColumn)
        {
            prevColumn = index.column();

            if(_gpxLayer != nullptr)
            {
                _gpxLayer->highlightSelectedPoint(_gpxId, index.row());
            }
        }
    }
}

void GpxInfoFileWidget::clearSelectedPoint()
{
    if(_gpxLayer != nullptr)
    {
        _gpxLayer->removeSelectedPoint(_gpxId);

        _ui->tableView->clearSelection();
    }
}

void GpxInfoFileWidget::changeSelectedPoint(int gpxId, size_t idx)
{
    if(gpxId == _gpxId)
    {
        QObject::disconnect(_ui->tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged(QItemSelection,QItemSelection)));

        _ui->tableView->selectRow(idx);

        QObject::connect(_ui->tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(selectionChanged(QItemSelection,QItemSelection)));
    }
}

void GpxInfoFileWidget::centerMap()
{
    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & item : gpxVector)
        {
            if(item.fileId == _gpxId)
            {
                QModelIndexList indexes = _ui->tableView->selectionModel()->selectedRows();

                if(indexes.size() > 0)
                {
                    QModelIndex index = indexes.at(0);

                    if(index.isValid() == true)
                    {
                        int row = index.row();

                        if(_gpxLayer != nullptr)
                        {
                            if(row < int(item.pointVector.size()))
                            {
                                QPoint pos = _gpxLayer->getPixelPoint(item.pointVector[row].lat, item.pointVector[row].lon);

                                emit centerMap(pos);
                            }
                        }
                    }
                }
            }
        }
    }
}
