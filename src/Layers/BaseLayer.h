#ifndef BASELAYER_H
#define BASELAYER_H

#include "MapSettings.h"

#include <QObject>
#include <QPaintEvent>

class BaseLayer
{
    public:
        explicit BaseLayer(MapSettings & mapSettings);

    public:
        virtual void paintEvent(QPainter & painter) = 0;
        virtual void wheelEvent(QWheelEvent * wheelEvent);
        virtual void mouseMoveEvent(QMouseEvent * mouseEvent);
        virtual void mousePressEvent(QMouseEvent * mouseEvent);
        virtual void keyPressEvent(QKeyEvent * keyEvent);

    protected:
        MapSettings & _mapSettings;
};

#endif // BASELAYER_H
