#ifndef GPXLAYER_H
#define GPXLAYER_H

#include "BaseLayer.h"

#include "../GpxManager.h"

class GpxLayer : public QWidget, public BaseLayer
{
        Q_OBJECT

    public:
        GpxLayer(const QString & layerName, MapSettings & mapSettings, QWidget *parent = 0);
        virtual ~GpxLayer() = default;

    public:
        void paintEvent(QPainter & painter);
        void mouseMoveEvent(QMouseEvent * mouseEvent);

        void setCurrentGpxIndexes(const GpxManager::GpxIdVector & ids);
        void setGpxManager(GpxManagerPtr gpxManager);

        void highlightSelectedPoint(int gpxId, size_t idx);
        void removeSelectedPoint(int gpxId);

        QPoint getPixelPoint(double lat, double lon);

    signals:
        void refreshView();
        void changeSelectedPoint(int gpxId, size_t idx);

    private:
        using SelectedPointsMap = std::map<int, size_t>; // int - ID GPX suboru, size_t - index vybrany v tabulke bodov pre dany GPX
        void paintPoint(QPainter & painter, const GpxManager::GpxItem & item, size_t posIdx, const QString & title = "");

    private:
        GpxManagerPtr _gpxManager;
        GpxManager::GpxIdVector _currentGpx;
        int _mouseX = -1;
        int _mouseY = -1;
        SelectedPointsMap _selectedPoints;
};

#endif // GPXLAYER_H
