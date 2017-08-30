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
        void setCenterPosition(const QPointF & center);
        MapSettings & getMapSettings();

        QString getOSMDirectoryPath();
        void setOSMDirectoryPath(QString path);

        void addLayer(BaseLayer * layer, QString layerName);

        class OSMLayer * getOSMLayer();

        QPointF getTopLeft();
        QPointF getBottomRight();

    signals:
        void mouseCursorWgsChanged(double lat, double lon);
        void downloadArea();
        void downloadSelectedArea(QPointF topLeft, QPointF bottomRight);

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

        enum SelectedAreaState
        {
            Unselecting,
            PrepareForSelecting,
            Selecting
        };

    private:
        void centerToWgs(QPointF wgsPoint);
        void centerToWgs(double lon, double lat);

    private slots:
        void startSelectArea();
        void centerMapToPixels(QPoint pos);

    private:
        Layers _layers;
        MapSettings _mapSettings;
        class MapContextMenu * _contextMenu = nullptr;
        QPoint _startPointSelectArea;
        QPoint _endPointSelectArea;
        SelectedAreaState _selectedAreaState;
};

#endif // PAINTINGWIDGET_H
