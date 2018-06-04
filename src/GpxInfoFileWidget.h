#ifndef GPXTABLEINFOFILEWIDGET_H
#define GPXTABLEINFOFILEWIDGET_H

#include <QWidget>

#include "GpxInfoFileModel.h"
#include "Layers/GpxLayer.h"

namespace Ui {
class GpxInfoFileWidget;
}

class GpxInfoFileWidget : public QWidget
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

    private:
        void fillTable();

    private:
        Ui::GpxInfoFileWidget * _ui = nullptr;
        GpxInfoFileModel * _tableModel = nullptr;
        GpxManager * _gpxManager = nullptr;
        int _gpxId = GpxManager::ErrorId;
        GpxLayer * _gpxLayer = nullptr;
};

#endif // GPXTABLEINFOFILEWIDGET_H
