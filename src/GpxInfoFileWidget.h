#ifndef GPXTABLEINFOFILEWIDGET_H
#define GPXTABLEINFOFILEWIDGET_H

#include <QWidget>

#include "GpxInfoFileModel.h"

namespace Ui {
class GpxInfoFileWidget;
}

class GpxInfoFileWidget : public QWidget
{
        Q_OBJECT

    public:
        GpxInfoFileWidget(GpxManager * gpxManager, int gpxId, QWidget *parent = 0);
        ~GpxInfoFileWidget();

    private slots:
        void selectMaxElevation();
        void selectMaxCadention();
        void selectMaxHeartRate();
        void selectMaxTemperature();

    private:
        void fillTable();

    private:
        Ui::GpxInfoFileWidget * _ui = nullptr;
        GpxInfoFileModel * _tableModel = nullptr;
        GpxManager * _gpxManager = nullptr;
        int _gpxId = GpxManager::ErrorId;
};

#endif // GPXTABLEINFOFILEWIDGET_H
