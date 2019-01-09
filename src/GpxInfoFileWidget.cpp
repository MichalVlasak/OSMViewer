#include "GpxInfoFileWidget.h"
#include "ui_GpxInfoFileWidget.h"

#include <QCheckBox>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

GpxInfoFileWidget::GpxInfoFileWidget(GpxManagerPtr gpxManager, GpxLayer * gpxLayer, int gpxId, QWidget *parent) :
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
    QObject::connect(_ui->cadentionGraphChck, SIGNAL(toggled(bool)), SLOT(cadentionShowGraphChecked(bool)));
    QObject::connect(_ui->elevationGraphChck, SIGNAL(toggled(bool)), SLOT(elevationShowGraphChecked(bool)));
    QObject::connect(_ui->heartRateGraphChck, SIGNAL(toggled(bool)), SLOT(heartRateShowGraphChecked(bool)));
    QObject::connect(_ui->temperatureGraphChck, SIGNAL(toggled(bool)), SLOT(temperatureShowGraphChecked(bool)));
    QObject::connect(_ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(changeVisibleTab(int)));

    QVBoxLayout *layout = new QVBoxLayout();

    _chart = new QtCharts::QChart();
    _chartView = new QtCharts::QChartView(_chart);

    _chartView->setRubberBand(QtCharts::QChartView::HorizontalRubberBand);
    _chartView->setRenderHint(QPainter::Antialiasing);

    _chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

    layout->addWidget(_chartView);
    layout->setMargin(0);

    _ui->graphWidget->setLayout(layout);

    fillTable();
    fillGraph();
    initializeGui();

    _ui->cadentionGraphChck->setChecked(CadentionShowGrapChck);
    cadentionShowGraphChecked(CadentionShowGrapChck);

    _ui->elevationGraphChck->setChecked(ElevationShowGraphChck);
    elevationShowGraphChecked(ElevationShowGraphChck);

    _ui->heartRateGraphChck->setChecked(HeartRateShowGraphChck);
    heartRateShowGraphChecked(HeartRateShowGraphChck);

    _ui->temperatureGraphChck->setChecked(TemperatureShowGraphChck);
    temperatureShowGraphChecked(TemperatureShowGraphChck);
}

GpxInfoFileWidget::~GpxInfoFileWidget()
{
    clearSelectedPoint();

    delete _ui;
}

void GpxInfoFileWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::MouseButton::LeftButton)
    {
        _chart->zoomReset();
        return;
    }

    QWidget::mousePressEvent(event);
}

void GpxInfoFileWidget::wheelEvent(QWheelEvent * wheelEvent)
{
    QWidget::wheelEvent(wheelEvent);

    if (wheelEvent->angleDelta().y() > 0)
    {
        zoomX(1.1, wheelEvent->pos().x() - _chart->plotArea().x());
        qWarning() << _chart->plotArea();
    }
    else if (wheelEvent->angleDelta().y() < 0)
    {
        zoomX(0.9, wheelEvent->pos().x() - _chart->plotArea().x());
    }
}

void GpxInfoFileWidget::zoomX(qreal factor, qreal xcenter)
{
    QRectF rect = _chart->plotArea();
    qreal widthOriginal = rect.width();
    rect.setWidth(widthOriginal / factor);
    qreal centerScale = (xcenter / widthOriginal);

    qreal leftOffset = (xcenter - (rect.width() * centerScale) );

    rect.moveLeft(rect.x() + leftOffset);
    _chart->zoomIn(rect);
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
    if(_chart != nullptr)
    {
        _elevationGraph_2 = new QtCharts::QLineSeries();
        _heartRateGraph_2 = new QtCharts::QLineSeries();
        _cadentionGraph_2 = new QtCharts::QLineSeries();
        _temperatureGraph_2 = new QtCharts::QLineSeries();

        //_chart->legend()->hide();
        _chart->createDefaultAxes();

        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(const GpxManager::GpxItem & item : gpxVector)
        {
            if(item.fileId == _gpxId)
            {
                _chart->setTitle(item.name);

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
                        startTime = item.pointVector[0].time.toDateTime().toMSecsSinceEpoch();
                    }
                }

                for(const GpxManager::Point & point : item.pointVector)
                {
                    if(point.time.isNull() == false)
                    {
                        double time = endTime = point.time.toDateTime().toMSecsSinceEpoch();

                        if(point.elevation.isNull() == false)
                        {
                            bool isOk = false;
                            double elevation = point.elevation.toDouble(&isOk);

                            if(isOk == true)
                            {
                                minElevation = std::min(elevation, minElevation);
                                maxElevation = std::max(elevation, maxElevation);

                                _elevationGraph_2->append(time, elevation);
                            }
                        }

                        if(point.heartRate.isNull() == false)
                        {
                            bool isOk = false;

                            double heartRate= point.heartRate.toInt(&isOk);

                            if(isOk == true)
                            {
                                minHeartRate = std::min(heartRate, minHeartRate);
                                maxHeartRate = std::max(heartRate, maxHeartRate);

                                _heartRateGraph_2->append(time, heartRate);
                            }
                        }

                        if(point.cadention.isNull() == false)
                        {
                            bool isOk = false;

                            double cadention = point.cadention.toInt(&isOk);

                            if(isOk == true)
                            {
                                minCadention = std::min(cadention, minCadention);
                                maxCadention = std::max(cadention, maxCadention);

                                _cadentionGraph_2->append(time, cadention);
                            }
                        }

                        if(point.temperature.isNull() == false)
                        {
                            bool isOk = false;

                            double temperature = point.temperature.toFloat(&isOk);

                            if(isOk == true)
                            {
                                minTemperature = std::min(temperature, minTemperature);
                                maxTemperature = std::max(temperature, maxTemperature);

                                _temperatureGraph_2->append(time, temperature);
                            }
                        }
                    }
                }

                QtCharts::QDateTimeAxis *axisX = new QtCharts::QDateTimeAxis;
                //axisX->setTickCount(8);
                axisX->setFormat("hh:mm:ss");
                axisX->setTitleText(tr("Time"));
                axisX->setMin(QDateTime::fromMSecsSinceEpoch(startTime));
                axisX->setMin(QDateTime::fromMSecsSinceEpoch(endTime));

                _chart->addAxis(axisX, Qt::AlignBottom);

                if(_elevationGraph_2->count() > 0)
                {
                    _chart->addSeries(_elevationGraph_2);

                    QtCharts::QValueAxis * axisYElevation = new QtCharts::QValueAxis;

                    axisYElevation->setLabelFormat("%i");
                    axisYElevation->setTitleText(tr("Elevation [m]"));
                    axisYElevation->setMin(minElevation * 0.9);
                    axisYElevation->setMax(maxElevation * 1.1);

                    _chart->addAxis(axisYElevation, Qt::AlignLeft);

                    _elevationGraph_2->attachAxis(axisYElevation);
                    _elevationGraph_2->setName(tr("Elevation"));
                    _elevationGraph_2->setColor(Qt::darkGreen);
                    _elevationGraph_2->attachAxis(axisX);

                    _ui->elevationGraphChck->setDisabled(false);
                }
                else
                {
                    _ui->elevationGraphChck->setDisabled(true);
                }

                if(_heartRateGraph_2->count() > 0)
                {
                    _chart->addSeries(_heartRateGraph_2);

                    QtCharts::QValueAxis * axisYHeartRate = new QtCharts::QValueAxis;

                    axisYHeartRate->setLabelFormat("%i");
                    axisYHeartRate->setTitleText(tr("Heart Rate [bpm]"));
                    axisYHeartRate->setMin(minHeartRate * 0.9);
                    axisYHeartRate->setMax(maxHeartRate * 1.1);

                    _chart->addAxis(axisYHeartRate, Qt::AlignRight);

                    _heartRateGraph_2->setName(tr("Heart Rate"));
                    _heartRateGraph_2->setColor(Qt::red);
                    _heartRateGraph_2->attachAxis(axisX);
                    _heartRateGraph_2->attachAxis(axisYHeartRate);

                    _ui->heartRateGraphChck->setDisabled(false);
                }
                else
                {
                    _ui->heartRateGraphChck->setDisabled(true);
                }

                if(_cadentionGraph_2->count() > 0)
                {
                    _chart->addSeries(_cadentionGraph_2);

                    QtCharts::QValueAxis * axisYCadention = new QtCharts::QValueAxis;

                    axisYCadention->setLabelFormat("%i");
                    axisYCadention->setTitleText(tr("Cadention"));
                    axisYCadention->setMin(minCadention * 0.9);
                    axisYCadention->setMax(maxCadention * 1.1);

                    _chart->addAxis(axisYCadention, Qt::AlignLeft);

                    _cadentionGraph_2->setName(tr("Cadention"));
                    _cadentionGraph_2->setColor(Qt::darkGray);
                    _cadentionGraph_2->attachAxis(axisX);
                    _cadentionGraph_2->attachAxis(axisYCadention);

                    _ui->cadentionGraphChck->setDisabled(false);
                }
                else
                {
                    _ui->cadentionGraphChck->setDisabled(true);
                }

                if(_temperatureGraph_2->count() > 0)
                {
                    _chart->addSeries(_temperatureGraph_2);

                    QtCharts::QValueAxis * axisYTemperature = new QtCharts::QValueAxis;

                    axisYTemperature->setLabelFormat("%i");
                    axisYTemperature->setTitleText("Temperature [°C]");
                    axisYTemperature->setMin(minTemperature * 0.9);
                    axisYTemperature->setMax(maxTemperature * 1.1);

                    _chart->addAxis(axisYTemperature, Qt::AlignRight);

                    _temperatureGraph_2->setName(tr("Temperature"));
                    _temperatureGraph_2->setColor(Qt::blue);
                    _temperatureGraph_2->attachAxis(axisX);
                    _temperatureGraph_2->attachAxis(axisYTemperature);

                    _ui->temperatureGraphChck->setDisabled(false);
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

    if(_cadentionGraph_2 != nullptr)
    {
        _cadentionGraph_2->setVisible(checked);
    }
}

void GpxInfoFileWidget::elevationShowGraphChecked(bool checked)
{
    ElevationShowGraphChck = checked;

    if(_elevationGraph_2 != nullptr)
    {
        _elevationGraph_2->setVisible(checked);
    }
}

void GpxInfoFileWidget::heartRateShowGraphChecked(bool checked)
{
    HeartRateShowGraphChck = checked;

    if(_heartRateGraph_2 != nullptr)
    {
        _heartRateGraph_2->setVisible(checked);
    }
}

void GpxInfoFileWidget::temperatureShowGraphChecked(bool checked)
{
    TemperatureShowGraphChck = checked;

    if(_temperatureGraph_2 != nullptr)
    {
        _temperatureGraph_2->setVisible(checked);
    }
}
