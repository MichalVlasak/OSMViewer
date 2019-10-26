#ifndef BASELAYER_H
#define BASELAYER_H

#include "MapSettings.h"

#include <QObject>
#include <QPaintEvent>

class BaseLayer
{
    public:
        explicit BaseLayer(const QString & layerName, MapSettings & mapSettings);
        ~BaseLayer() = default;

    public:
        virtual void paintEvent(QPainter & painter) = 0;
        virtual void wheelEvent(QWheelEvent * wheelEvent);
        virtual void mouseMoveEvent(QMouseEvent * mouseEvent);
        virtual void mousePressEvent(QMouseEvent * mouseEvent);
        virtual void keyPressEvent(QKeyEvent * keyEvent);

        void setVisible(bool visible);
        bool isVisible() const;

        const QString & getLayerName() const;

    protected:
        MapSettings & _mapSettings;
        QString _layerName;
        bool _isVisible;
};

Q_DECLARE_METATYPE(BaseLayer*)

#endif // BASELAYER_H
