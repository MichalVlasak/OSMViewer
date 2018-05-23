#ifndef GPXLAYER_H
#define GPXLAYER_H

#include "BaseLayer.h"

#include "../GpxManager.h"

class GpxLayer : public QWidget, public BaseLayer
{
        Q_OBJECT

    public:
        GpxLayer(MapSettings & mapSettings, QWidget *parent = 0);
        virtual ~GpxLayer() = default;

    public:
        void paintEvent(QPainter & painter);
        void mouseMoveEvent(QMouseEvent * mouseEvent);

        void setCurrentGpxIndexes(const GpxManager::GpxIdVector & ids);
        void setGpxManager(GpxManager * gpxManager);

    signals:
        void refreshView();

    private:
        GpxManager * _gpxManager = nullptr;
        GpxManager::GpxIdVector _currentGpx;
        int _mouseX = -1;
        int _mouseY = -1;
};

#endif // GPXLAYER_H
