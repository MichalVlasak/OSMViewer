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
        virtual void paintEvent(QPaintEvent * paintEvent) = 0;
        void wheelEvent(QWheelEvent * wheelEvent);
        void mouseMoveEvent(QMouseEvent * mouseEvent);
        void mousePressEvent(QMouseEvent * mouseEvent);
        void keyPressEvent(QKeyEvent * keyEvent);

    protected:
        MapSettings & _mapSettings;
};

#endif // BASELAYER_H
