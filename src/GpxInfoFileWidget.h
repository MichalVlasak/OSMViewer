#ifndef GPXTABLEINFOFILEWIDGET_H
#define GPXTABLEINFOFILEWIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QGestureEvent>

#include "GpxInfoFileModel.h"
#include "GpxInfoFileWidgetConfig.h"
#include "Layers/GpxLayer.h"

namespace Ui {
class GpxInfoFileWidget;
}

class GpxInfoFileWidget : public QWidget, public GpxInfoFileWidgetConfig
{
        Q_OBJECT

    public:
        GpxInfoFileWidget(GpxManager * gpxManager, GpxLayer * gpxLayer, int gpxId, QWidget *parent = 0);
        ~GpxInfoFileWidget();

    signals:
        void centerMap(QPoint pos);

    public slots:
        void clearSelectedPoint();

    private slots:
        void selectMaxElevation();
        void selectMaxCadention();
        void selectMaxHeartRate();
        void selectMaxTemperature();
        void selectionChanged(QItemSelection selected, QItemSelection deselected);
        void changeSelectedPoint(int gpxId, size_t idx);
        void centerMap();
        void cadentionShowMaxChecked(bool checked);
        void elevationShowMaxChecked(bool checked);
        void heartRateShowMaxChecked(bool checked);
        void temperatureShowMaxChecked(bool checked);
        void cadentionShowGraphChecked(bool checked);
        void elevationShowGraphChecked(bool checked);
        void heartRateShowGraphChecked(bool checked);
        void temperatureShowGraphChecked(bool checked);
        void changeVisibleTab(int index);

    private:
        void initializeGui();
        void fillTable();
        void fillGraph();
        void setShowingBigestElevation(GpxManager::GpxItem & item);
        void setShowingBigestCadention(GpxManager::GpxItem & item);
        void setShowingBigestHeartRate(GpxManager::GpxItem & item);
        void setShowingBigestTemperature(GpxManager::GpxItem & item);

        void wheelEvent(QWheelEvent * wheelEvent);
        void zoomX(qreal factor, qreal xcenter);

    private:
        Ui::GpxInfoFileWidget * _ui = nullptr;
        GpxInfoFileModel * _tableModel = nullptr;
        GpxManager * _gpxManager = nullptr;
        int _gpxId = GpxManager::ErrorId;
        GpxLayer * _gpxLayer = nullptr;
        QtCharts::QChartView  * _chartView = nullptr;
        QtCharts::QChart      * _chart = nullptr;
        QtCharts::QLineSeries * _elevationGraph_2 = nullptr;
        QtCharts::QLineSeries * _heartRateGraph_2 = nullptr;
        QtCharts::QLineSeries * _cadentionGraph_2 = nullptr;
        QtCharts::QLineSeries * _temperatureGraph_2 = nullptr;
};

#endif // GPXTABLEINFOFILEWIDGET_H
