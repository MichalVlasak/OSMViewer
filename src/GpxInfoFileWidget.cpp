#include "GpxInfoFileWidget.h"
#include "ui_GpxInfoFileWidget.h"

#include "external/qcustomplot/qcustomplot.h"

#include <QCheckBox>

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
    QObject::connect(_ui->cadentionChck, SIGNAL(clicked(bool)), SLOT(cadentionChecked(bool)));
    QObject::connect(_ui->elevationChck, SIGNAL(clicked(bool)), SLOT(elevationChecked(bool)));
    QObject::connect(_ui->heartRateChck, SIGNAL(clicked(bool)), SLOT(heartRateChecked(bool)));
    QObject::connect(_ui->temperatureChck, SIGNAL(clicked(bool)), SLOT(temperatureChecked(bool)));
    QObject::connect(_ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(changeVisibleTab(int)));

    QHBoxLayout *layout = new QHBoxLayout();
    _plot = new QCustomPlot(this);

    layout->addWidget(_plot);

    _ui->graphTab->setLayout(layout);

    fillTable();
    fillGraph();
    initializeGui();
}

GpxInfoFileWidget::~GpxInfoFileWidget()
{
    clearSelectedPoint();

    delete _ui;
}

void GpxInfoFileWidget::initializeGui()
{
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
                    _ui->cadentionChck->setDisabled(true);
                }
                else
                {
                    GpxManager::GpxItem & itemNoConst = const_cast<GpxManager::GpxItem &>(item);

                    setShowingBigestCadention(itemNoConst);

                    _ui->cadentionChck->setChecked(CadentionCheckChck);
                }

                if(item.biggestElevetionIdx == -1)
                {
                    _ui->maxElevation->setDisabled(true);
                    _ui->elevationChck->setDisabled(true);
                }
                else
                {
                    GpxManager::GpxItem & itemNoConst = const_cast<GpxManager::GpxItem &>(item);

                    setShowingBigestElevation(itemNoConst);

                    _ui->elevationChck->setChecked(ElevationCheckChck);
                }

                if(item.biggestHeartRateIdx == -1)
                {
                    _ui->maxHeartRate->setDisabled(true);
                    _ui->heartRateChck->setDisabled(true);
                }
                else
                {
                    GpxManager::GpxItem & itemNoConst = const_cast<GpxManager::GpxItem &>(item);

                    setShowingBigestHeartRate(itemNoConst);

                    _ui->heartRateChck->setChecked(HeartRateCheckChck);
                }

                if(item.biggestTemperatureIdx == -1)
                {
                    _ui->maxTemperature->setDisabled(true);
                    _ui->temperatureChck->setDisabled(true);
                }
                else
                {
                    GpxManager::GpxItem & itemNoConst = const_cast<GpxManager::GpxItem &>(item);

                    setShowingBigestTemperature(itemNoConst);

                    _ui->temperatureChck->setChecked(TemperatureCheckChck);
                }
            }
        }
    }

    _ui->clearSelection->setDisabled(true);
    _ui->center->setDisabled(true);

    switch (TabWidgetType)
    {
        case TabWidgetTypeEnum::Graph:
            _ui->tabWidget->setCurrentWidget(_ui->graphTab);
            break;

        case TabWidgetTypeEnum::Table:
        default:
            _ui->tabWidget->setCurrentWidget(_ui->tableTab);
            break;
    }
}

void GpxInfoFileWidget::fillGraph()
{
    if(_plot != nullptr && _gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & item : gpxVector)
        {
            if(item.fileId == _gpxId)
            {
                QSharedPointer<QCPGraphDataContainer> elevationContainer = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer());
                QSharedPointer<QCPGraphDataContainer> heartRateContainer = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer());

                double startTime;
                double endTime;
                double minElevation = std::numeric_limits<double>::max();
                double maxElevation = 0;
                double minHeartRate = std::numeric_limits<double>::max();
                double maxHeartRate = 0;

                if(item.pointVector.size() > 0)
                {
                    if(item.pointVector[0].time.isNull() == false)
                    {
                        startTime = item.pointVector[0].time.toDateTime().toTime_t();
                    }
                }

                for(const GpxManager::Point & point : item.pointVector)
                {
                    if(point.time.isNull() == false)
                    {
                        double time = endTime = point.time.toDateTime().toTime_t();

                        if(point.elevation.isNull() == false)
                        {
                            QCPGraphData elevationData;
                            QCPGraphData heartRateData;

                            bool isOk = false;

                            elevationData.key = time;
                            elevationData.value = point.elevation.toDouble(&isOk);

                            if(isOk == true)
                            {
                                minElevation = std::min(elevationData.value, minElevation);
                                maxElevation = std::max(elevationData.value, maxElevation);

                                elevationContainer->add(elevationData);
                            }

                            heartRateData.key = time;
                            heartRateData.value = point.heartRate.toInt(&isOk);

                            if(isOk == true)
                            {
                                minHeartRate = std::min(heartRateData.value, minHeartRate);
                                maxHeartRate = std::max(heartRateData.value, maxHeartRate);

                                heartRateContainer->add(heartRateData);
                            }
                        }

                        if(point.heartRate.isNull() == false)
                        {
                            QCPGraphData elevationData;
                            QCPGraphData heartRateData;

                            bool isOk = false;

                            elevationData.key = time;
                            elevationData.value = point.elevation.toDouble(&isOk);

                            if(isOk == true)
                            {
                                minElevation = std::min(elevationData.value, minElevation);
                                maxElevation = std::max(elevationData.value, maxElevation);

                                elevationContainer->add(elevationData);
                            }

                            heartRateData.key = time;
                            heartRateData.value = point.heartRate.toInt(&isOk);

                            if(isOk == true)
                            {
                                minHeartRate = std::min(heartRateData.value, minHeartRate);
                                maxHeartRate = std::max(heartRateData.value, maxHeartRate);

                                heartRateContainer->add(heartRateData);
                            }
                        }
                    }
                }

                _plot->xAxis->setLabel("Time");

                // configure bottom axis to show date instead of number:
                QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
                dateTicker->setDateTimeFormat("hh:mm:ss");
                _plot->xAxis->setTicker(dateTicker);

                _plot->legend->setVisible(true);
                _plot->legend->setBrush(QColor(255, 255, 255, 150));

                if(elevationContainer.data() != nullptr && elevationContainer.data()->size() > 0)
                {
                    QCPGraph * elevationGraph = _plot->addGraph();

                    if(elevationGraph != nullptr)
                    {
                        QColor color(Qt::darkGreen);

                        elevationGraph->setPen(QPen(color));
                        elevationGraph->setName(tr("Elevation"));
                        elevationGraph->setData(elevationContainer);
                    }

                    // set a more compact font size for bottom and left axis tick labels:
                    _plot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
                    _plot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

                    _plot->xAxis->setRange(startTime, endTime);
                    _plot->yAxis->setRange(minElevation * 0.9, maxElevation * 1.1);
                }

                if(heartRateContainer.data() != nullptr && heartRateContainer.data()->size() > 0)
                {
                    QCPGraph * heartRateGraph = _plot->addGraph(_plot->xAxis, _plot->yAxis2);

                    if(heartRateGraph != nullptr)
                    {
                        QColor color(Qt::red);

                        heartRateGraph->setPen(QPen(color));
                        heartRateGraph->setName(tr("Heart Rate"));
                        heartRateGraph->setData(heartRateContainer);
                    }

                    _plot->yAxis2->setVisible(true);
                    _plot->yAxis2->setTicks(true);
                    _plot->yAxis2->setTickLabels(true);
                    _plot->yAxis2->setRange(minHeartRate * 0.9, maxHeartRate * 1.1);
                }
            }
        }
    }
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

            int row = index.row();
            bool isShowed = false;

            if(_gpxManager != nullptr)
            {
                const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

                for(const GpxManager::GpxItem & item : gpxVector)
                {
                    if(item.fileId == _gpxId)
                    {
                        if((item.biggestCadentionIdx == row && item.showBiggestCadention == true) ||
                           (item.biggestElevetionIdx == row && item.showBiggestElevetion == true) ||
                           (item.biggestHeartRateIdx == row && item.showBiggestHeartRate == true) ||
                           (item.biggestTemperatureIdx == row && item.showBiggestTemperature == true))
                        {
                            isShowed = true;
                        }
                    }
                }
            }

            if(_gpxLayer != nullptr && isShowed == false)
            {
                _gpxLayer->highlightSelectedPoint(_gpxId, row);
            }
        }
    }

    if(indexes.size() > 0)
    {
        _ui->clearSelection->setDisabled(false);
        _ui->center->setDisabled(false);
    }
    else
    {
        _ui->clearSelection->setDisabled(true);
        _ui->center->setDisabled(true);
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

void GpxInfoFileWidget::elevationChecked(bool checked)
{
    ElevationCheckChck = checked;

    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & item : gpxVector)
        {
            if(item.fileId == _gpxId)
            {
                GpxManager::GpxItem & itemNoConst = const_cast<GpxManager::GpxItem &>(item);

                setShowingBigestElevation(itemNoConst);
            }
        }
    }
}

void GpxInfoFileWidget::setShowingBigestElevation(GpxManager::GpxItem & item)
{
    item.showBiggestElevetion = ElevationCheckChck;

    if(_gpxLayer != nullptr)
    {
        _gpxLayer->refreshView();
    }
}

void GpxInfoFileWidget::cadentionChecked(bool checked)
{
    CadentionCheckChck = checked;

    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & item : gpxVector)
        {
            if(item.fileId == _gpxId)
            {
                GpxManager::GpxItem & itemNoConst = const_cast<GpxManager::GpxItem &>(item);

                setShowingBigestCadention(itemNoConst);
            }
        }
    }
}

void GpxInfoFileWidget::setShowingBigestCadention(GpxManager::GpxItem & item)
{
    item.showBiggestCadention = CadentionCheckChck;

    if(_gpxLayer != nullptr)
    {
        _gpxLayer->refreshView();
    }
}

void GpxInfoFileWidget::temperatureChecked(bool checked)
{
    TemperatureCheckChck = checked;

    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & item : gpxVector)
        {
            if(item.fileId == _gpxId)
            {
                GpxManager::GpxItem & itemNoConst = const_cast<GpxManager::GpxItem &>(item);

                setShowingBigestTemperature(itemNoConst);
            }
        }
    }
}

void GpxInfoFileWidget::setShowingBigestTemperature(GpxManager::GpxItem & item)
{
    item.showBiggestTemperature = TemperatureCheckChck;

    if(_gpxLayer != nullptr)
    {
        _gpxLayer->refreshView();
    }
}

void GpxInfoFileWidget::heartRateChecked(bool checked)
{
    HeartRateCheckChck = checked;

    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & item : gpxVector)
        {
            if(item.fileId == _gpxId)
            {
                GpxManager::GpxItem & itemNoConst = const_cast<GpxManager::GpxItem &>(item);

                setShowingBigestHeartRate(itemNoConst);
            }
        }
    }
}

void GpxInfoFileWidget::setShowingBigestHeartRate(GpxManager::GpxItem & item)
{
    item.showBiggestHeartRate = HeartRateCheckChck;

    if(_gpxLayer != nullptr)
    {
        _gpxLayer->refreshView();
    }
}

void GpxInfoFileWidget::changeVisibleTab(int index)
{
    if(_ui->tabWidget->currentWidget() == _ui->tableTab)
    {
        TabWidgetType = TabWidgetTypeEnum::Table;
    }
    else if(_ui->tabWidget->currentWidget() == _ui->graphTab)
    {
        TabWidgetType = TabWidgetTypeEnum::Graph;
    }
}
