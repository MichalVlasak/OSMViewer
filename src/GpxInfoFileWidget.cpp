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
    QObject::connect(_ui->cadentionShowMaxChck, SIGNAL(clicked(bool)), SLOT(cadentionShowMaxChecked(bool)));
    QObject::connect(_ui->elevationShowMaxChck, SIGNAL(clicked(bool)), SLOT(elevationShowMaxChecked(bool)));
    QObject::connect(_ui->heartRateShowMaxChck, SIGNAL(clicked(bool)), SLOT(heartRateShowMaxChecked(bool)));
    QObject::connect(_ui->temperatureShowMaxChck, SIGNAL(clicked(bool)), SLOT(temperatureShowMaxChecked(bool)));
    QObject::connect(_ui->cadentionGraphChck, SIGNAL(clicked(bool)), SLOT(cadentionShowGraphChecked(bool)));
    QObject::connect(_ui->elevationGraphChck, SIGNAL(clicked(bool)), SLOT(elevationShowGraphChecked(bool)));
    QObject::connect(_ui->heartRateGraphChck, SIGNAL(clicked(bool)), SLOT(heartRateShowGraphChecked(bool)));
    QObject::connect(_ui->temperatureGraphChck, SIGNAL(clicked(bool)), SLOT(temperatureShowGraphChecked(bool)));
    QObject::connect(_ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(changeVisibleTab(int)));

    QHBoxLayout *layout = new QHBoxLayout();
    _plot = new QCustomPlot(this);

    layout->addWidget(_plot);
    layout->setMargin(0);

    _ui->graphWidget->setLayout(layout);

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
    _ui->cadentionGraphChck->setChecked(CadentionShowGrapChck);
    _ui->elevationGraphChck->setChecked(ElevationShowGraphChck);
    _ui->heartRateGraphChck->setChecked(HeartRateShowGraphChck);
    _ui->temperatureGraphChck->setChecked(TemperatureShowGraphChck);

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
                    _ui->cadentionShowMaxChck->setDisabled(true);
                }
                else
                {
                    GpxManager::GpxItem & itemNoConst = const_cast<GpxManager::GpxItem &>(item);

                    setShowingBigestCadention(itemNoConst);

                    _ui->cadentionShowMaxChck->setChecked(CadentionShowMaxChck);
                }

                if(item.biggestElevetionIdx == -1)
                {
                    _ui->maxElevation->setDisabled(true);
                    _ui->elevationShowMaxChck->setDisabled(true);
                }
                else
                {
                    GpxManager::GpxItem & itemNoConst = const_cast<GpxManager::GpxItem &>(item);

                    setShowingBigestElevation(itemNoConst);

                    _ui->elevationShowMaxChck->setChecked(ElevationShowMaxChck);
                }

                if(item.biggestHeartRateIdx == -1)
                {
                    _ui->maxHeartRate->setDisabled(true);
                    _ui->heartRateShowMaxChck->setDisabled(true);
                }
                else
                {
                    GpxManager::GpxItem & itemNoConst = const_cast<GpxManager::GpxItem &>(item);

                    setShowingBigestHeartRate(itemNoConst);

                    _ui->heartRateShowMaxChck->setChecked(HeartRateShowMaxChck);
                }

                if(item.biggestTemperatureIdx == -1)
                {
                    _ui->maxTemperature->setDisabled(true);
                    _ui->temperatureShowMaxChck->setDisabled(true);
                }
                else
                {
                    GpxManager::GpxItem & itemNoConst = const_cast<GpxManager::GpxItem &>(item);

                    setShowingBigestTemperature(itemNoConst);

                    _ui->temperatureShowMaxChck->setChecked(TemperatureShowMaxChck);
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
        _plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        _plot->xAxis->axisRect()->setRangeDragAxes(_plot->xAxis, nullptr);
        _plot->xAxis->axisRect()->setRangeZoom(Qt::Horizontal);

        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & item : gpxVector)
        {
            if(item.fileId == _gpxId)
            {
                QSharedPointer<QCPGraphDataContainer> elevationContainer = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer());
                QSharedPointer<QCPGraphDataContainer> heartRateContainer = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer());
                QSharedPointer<QCPGraphDataContainer> cadentionContainer = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer());
                QSharedPointer<QCPGraphDataContainer> temperatureContainer = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer());

                double startTime;
                double endTime;
                double minElevation = std::numeric_limits<double>::max();
                double maxElevation = 0;
                double minHeartRate = std::numeric_limits<double>::max();
                double maxHeartRate = 0;
                double minCadention = std::numeric_limits<double>::max();
                double maxCadention = 0;
                double minTemperature = std::numeric_limits<double>::max();
                double maxTemperature = 0;

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

                            bool isOk = false;

                            elevationData.key = time;
                            elevationData.value = point.elevation.toDouble(&isOk);

                            if(isOk == true)
                            {
                                minElevation = std::min(elevationData.value, minElevation);
                                maxElevation = std::max(elevationData.value, maxElevation);

                                elevationContainer->add(elevationData);
                            }
                        }

                        if(point.heartRate.isNull() == false)
                        {
                            QCPGraphData heartRateData;

                            bool isOk = false;

                            heartRateData.key = time;
                            heartRateData.value = point.heartRate.toInt(&isOk);

                            if(isOk == true)
                            {
                                minHeartRate = std::min(heartRateData.value, minHeartRate);
                                maxHeartRate = std::max(heartRateData.value, maxHeartRate);

                                heartRateContainer->add(heartRateData);
                            }
                        }

                        if(point.cadention.isNull() == false)
                        {
                            QCPGraphData cadentionData;

                            bool isOk = false;

                            cadentionData.key = time;
                            cadentionData.value = point.cadention.toInt(&isOk);

                            if(isOk == true)
                            {
                                minCadention = std::min(cadentionData.value, minCadention);
                                maxCadention = std::max(cadentionData.value, maxCadention);

                                cadentionContainer->add(cadentionData);
                            }
                        }

                        if(point.temperature.isNull() == false)
                        {
                            QCPGraphData temperatureData;

                            bool isOk = false;

                            temperatureData.key = time;
                            temperatureData.value = point.temperature.toFloat(&isOk);

                            if(isOk == true)
                            {
                                minTemperature = std::min(temperatureData.value, minTemperature);
                                maxTemperature = std::max(temperatureData.value, maxTemperature);

                                temperatureContainer->add(temperatureData);
                            }
                        }
                    }
                }

                _plot->xAxis->setLabel(tr("Time"));
                _plot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));

                // configure bottom axis to show date instead of number:
                QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
                dateTicker->setDateTimeFormat("hh:mm:ss");
                _plot->xAxis->setTicker(dateTicker);

                _plot->legend->setVisible(true);
                _plot->legend->setBrush(QColor(255, 255, 255, 150));

                if(elevationContainer.data() != nullptr && elevationContainer.data()->size() > 0 )
                {
                    _elevationGraph = _plot->addGraph();

                    if(_elevationGraph != nullptr)
                    {
                        QColor color(Qt::darkGreen);

                        _elevationGraph->setPen(QPen(color));
                        _elevationGraph->setName(tr("Elevation"));
                        _elevationGraph->setData(elevationContainer);
                    }

                    _plot->yAxis->setLabel(tr("Elevation [m]"));
                    _plot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
                    _plot->xAxis->setRange(startTime, endTime);
                    _plot->yAxis->setRange(minElevation * 0.9, maxElevation * 1.1);

                    _elevationGraph->setVisible(ElevationShowGraphChck);

                    _ui->elevationGraphChck->setDisabled(false);
                }
                else
                {
                    _ui->elevationGraphChck->setDisabled(true);
                }

                if(heartRateContainer.data() != nullptr && heartRateContainer.data()->size() > 0)
                {
                    _heartRateGraph = _plot->addGraph(_plot->xAxis, _plot->yAxis2);

                    if(_heartRateGraph != nullptr)
                    {
                        QColor color(Qt::red);

                        _heartRateGraph->setPen(QPen(color));
                        _heartRateGraph->setName(tr("Heart Rate"));
                        _heartRateGraph->setData(heartRateContainer);
                    }

                    _plot->yAxis2->setLabel(tr("Heart Rate [bpm]"));
                    _plot->yAxis2->setTickLabelFont(QFont(QFont().family(), 8));
                    _plot->yAxis2->setVisible(true);
                    _plot->yAxis2->setTicks(true);
                    _plot->yAxis2->setTickLabels(true);
                    _plot->yAxis2->setRange(minHeartRate * 0.9, maxHeartRate * 1.1);

                    _heartRateGraph->setVisible(HeartRateShowGraphChck);

                    _ui->heartRateGraphChck->setDisabled(false);
                }
                else
                {
                    _ui->heartRateGraphChck->setDisabled(true);
                }

                if(cadentionContainer.data() != nullptr && cadentionContainer.data()->size() > 0)
                {
                    QList<QCPAxis*> newAxis = _plot->axisRect()->addAxes(QCPAxis::atLeft);

                    if(newAxis.size() > 0)
                    {
                        QCPAxis * y3 = newAxis.at(0);

                        y3->setLabel(tr("Cadention"));
                        y3->setTickLabelFont(QFont(QFont().family(), 8));
                        y3->setVisible(true);
                        y3->setTicks(true);
                        y3->setTickLabels(true);
                        y3->setRange(minCadention * 0.9, maxCadention * 1.1);

                        _cadentionGraph = _plot->addGraph(_plot->xAxis, y3);

                        if(_cadentionGraph != nullptr)
                        {
                            QColor color(Qt::darkGray);

                            _cadentionGraph->setPen(QPen(color));
                            _cadentionGraph->setName(tr("Cadention"));
                            _cadentionGraph->setData(cadentionContainer);
                        }

                        _cadentionGraph->setVisible(CadentionShowGrapChck);

                        _ui->cadentionGraphChck->setDisabled(false);
                    }
                }
                else
                {
                    _ui->cadentionGraphChck->setDisabled(true);
                }

                if(temperatureContainer.data() != nullptr && temperatureContainer.data()->size() > 0)
                {
                    QList<QCPAxis*> newAxis = _plot->axisRect()->addAxes(QCPAxis::atRight);

                    if(newAxis.size() > 0)
                    {
                        QCPAxis * y4 = newAxis.at(0);

                        y4->setLabel(tr("Temperature [°C]"));
                        y4->setTickLabelFont(QFont(QFont().family(), 8));
                        y4->setVisible(true);
                        y4->setTicks(true);
                        y4->setTickLabels(true);
                        y4->setRange(minTemperature * 0.9, maxTemperature * 1.1);

                        _temperatureGraph = _plot->addGraph(_plot->xAxis, y4);

                        if(_temperatureGraph != nullptr)
                        {
                            QColor color(Qt::blue);

                            _temperatureGraph->setPen(QPen(color));
                            _temperatureGraph->setName(tr("Temperature"));
                            _temperatureGraph->setData(temperatureContainer);
                        }

                        _temperatureGraph->setVisible(TemperatureShowGraphChck);

                        _ui->temperatureGraphChck->setDisabled(false);
                    }
                }
                else
                {
                    _ui->temperatureGraphChck->setDisabled(true);
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

void GpxInfoFileWidget::elevationShowMaxChecked(bool checked)
{
    ElevationShowMaxChck = checked;

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
    item.showBiggestElevetion = ElevationShowMaxChck;

    if(_gpxLayer != nullptr)
    {
        _gpxLayer->refreshView();
    }
}

void GpxInfoFileWidget::cadentionShowMaxChecked(bool checked)
{
    CadentionShowMaxChck = checked;

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
    item.showBiggestCadention = CadentionShowMaxChck;

    if(_gpxLayer != nullptr)
    {
        _gpxLayer->refreshView();
    }
}

void GpxInfoFileWidget::temperatureShowMaxChecked(bool checked)
{
    TemperatureShowMaxChck = checked;

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
    item.showBiggestTemperature = TemperatureShowMaxChck;

    if(_gpxLayer != nullptr)
    {
        _gpxLayer->refreshView();
    }
}

void GpxInfoFileWidget::heartRateShowMaxChecked(bool checked)
{
    HeartRateShowMaxChck = checked;

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
    item.showBiggestHeartRate = HeartRateShowMaxChck;

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

void GpxInfoFileWidget::cadentionShowGraphChecked(bool checked)
{
    CadentionShowGrapChck = checked;

    if(_cadentionGraph != nullptr)
    {
        _cadentionGraph ->setVisible(checked);

        if(_plot != nullptr)
        {
            _plot->replot();
        }
    }
}

void GpxInfoFileWidget::elevationShowGraphChecked(bool checked)
{
    ElevationShowGraphChck = checked;

    if(_elevationGraph != nullptr)
    {
        _elevationGraph->setVisible(checked);

        if(_plot != nullptr)
        {
            _plot->replot();
        }
    }
}

void GpxInfoFileWidget::heartRateShowGraphChecked(bool checked)
{
    HeartRateShowGraphChck = checked;

    if(_heartRateGraph != nullptr)
    {
        _heartRateGraph->setVisible(checked);

        if(_plot != nullptr)
        {
            _plot->replot();
        }
    }
}

void GpxInfoFileWidget::temperatureShowGraphChecked(bool checked)
{
    TemperatureShowGraphChck = checked;

    if(_temperatureGraph != nullptr)
    {
        _temperatureGraph->setVisible(checked);

        if(_plot != nullptr)
        {
            _plot->replot();
        }
    }
}
