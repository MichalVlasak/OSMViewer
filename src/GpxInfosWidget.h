#ifndef GPXINFOSWIDGET_H
#define GPXINFOSWIDGET_H

#include <QWidget>

#include "GpxInfoFileModel.h"
#include "Layers/GpxLayer.h"

namespace Ui {
class GpxInfosWidget;
}

class GpxInfosWidget : public QWidget
{
        Q_OBJECT

    public:
        GpxInfosWidget(GpxManager * gpxManager, GpxLayer * gpxLayer, QWidget *parent = 0);
        ~GpxInfosWidget();

    public slots:
        void changeSelectedGps(const GpxManager::GpxIdVector & ids);
        void deleteGpx(int id);
        void deleteAll();

    signals:
        void centerMap(QPoint pos);

    private:
        Ui::GpxInfosWidget * _ui;
        GpxManager * _gpxManager = nullptr;
        GpxManager::GpxIdVector _showedGpxIds;
        GpxLayer * _gpxLayer = nullptr;
};

#endif // GPXINFOSWIDGET_H
