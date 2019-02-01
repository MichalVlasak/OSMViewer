#ifndef PAINTINGWIDGET_H
#define PAINTINGWIDGET_H

#include <QWidget>

#include "BaseLayer.h"
#include "MapSettings.h"
#include "src/CenterPointStruct.h"
#include "src/hmi/AreaGeometry.h"

class PaintingWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit PaintingWidget(QWidget *parent = 0);
        virtual ~PaintingWidget();

    public:
        void setCenterPosition(const QPointF & center);
        MapSettings & getMapSettings();

        QString getOSMDirectoryPath();
        void setOSMDirectoryPath(QString path);

        void addLayer(BaseLayer * layer, QString layerName);

        class OSMLayer * getOSMLayer();
        class GpxLayer * getGpxLayer();

        QPointF getTopLeft();
        QPointF getBottomRight();

        void centerToPoint(const CenterPointStruct & centerPoint);

    signals:
        void mouseCursorWgsChanged(double lat, double lon);
        void downloadArea();
        void downloadSelectedArea(AreaGeometry geometry);
        void findInSelectedArea(AreaGeometry geometry);

    public slots:

    public:
        void paintEvent(QPaintEvent * paintEvent);
        void wheelEvent(QWheelEvent * wheelEvent);
        void mouseMoveEvent(QMouseEvent * mouseEvent);
        void mousePressEvent(QMouseEvent * mouseEvent);
        void mouseDoubleClickEvent(QMouseEvent * mouseEvent);
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
            PrepareForSelectAndDownloadRec,
            PrepareForSelectAndDownloadPoly,
            PrepareForSelectAndDownloadLine,
            PrepareForSelectAndFindRec,
            PrepareForSelectAndFindPoly,
            SelectAndDownloadRec,
            SelectAndDownloadPoly,
            SelectAndDownloadLine,
            SelectAndFindRec,
            SelectAndFindPoly
        };

    private:
        void centerToWgs(double lon, double lat);
        QPointF getWgsPointFromPixelsPoint(const QPoint & point);

    private slots:
        void startSelectAndDownloadAreaRec();
        void startSelectAndDownloadAreaPoly();
        void startSelectAndDownloadAreaLine();
        void startSelectAndFindAreaRec();
        void startSelectAndFindAreaPoly();
        void centerMapToPixels(QPoint pos);
        void downloadViewedAreaSlot();

    private:
        Layers _layers;
        MapSettings _mapSettings;
        class MapContextMenu * _contextMenu = nullptr;
        QPoint _startPointSelectRectangle;
        QPoint _endPointSelectRectangle;
        SelectedAreaState _selectedAreaState;
        class OSMLayer * _osmLayer = nullptr;
        class GpxLayer * _gpxLayer = nullptr;
        class GridLayer * _gridLayer = nullptr;
        QPolygonF _selectedPolygon;
        QPolygonF _selectedLine;
        AreaGeometry _selectGeometry;
};

#endif // PAINTINGWIDGET_H
