#ifndef PAINTINGWIDGET_H
#define PAINTINGWIDGET_H

#include <QWidget>

#include "BaseLayer.h"
#include "MapSettings.h"

class PaintingWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit PaintingWidget(QWidget *parent = 0);

    public:
        QPointF getCenterPosition();
        void setCenterPosition();
        void setCenterPosition(const QPointF & center);
        MapSettings & getMapSettings();

        QString getOSMDirectoryPath();
        void setOSMDirectoryPath(QString path);

        class OSMLayer * getOSMLayer();

        QPointF getTopLeft();
        QPointF getBottomRight();

    signals:
        void mouseCursorWgsChanged(double lat, double lon);
        void downloadArea();

    public slots:

    public:
        void paintEvent(QPaintEvent * paintEvent);
        void wheelEvent(QWheelEvent * wheelEvent);
        void mouseMoveEvent(QMouseEvent * mouseEvent);
        void mousePressEvent(QMouseEvent * mouseEvent);
        void keyPressEvent(QKeyEvent * keyEvent);


    private:
        struct LayerInfo
        {
                LayerInfo(BaseLayer * l, const QString s)
                {
                    layer = l;
                    name = s;
                }

                BaseLayer * layer = nullptr;
                QString name;
        };

        typedef std::vector<LayerInfo> Layers;

    private:
        void centerAfterZoom(double lon, double lat);

    private:
        Layers _layers;
        MapSettings _mapSettings;
        class MapContextMenu * _contextMenu = nullptr;
};

#endif // PAINTINGWIDGET_H
