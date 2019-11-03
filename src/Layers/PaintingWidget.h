#ifndef PAINTINGWIDGET_H
#define PAINTINGWIDGET_H

#include <QWidget>

#include "BaseLayer.h"
#include "MapSettings.h"
#include "src/CenterPointStruct.h"
#include "src/hmi/AreaGeometry.h"

class PaintingWidget : public QWidget, public StoreConfigInterface
{
        Q_OBJECT
    public:
        explicit PaintingWidget(QWidget *parent = 0);
        virtual ~PaintingWidget();

        using Layers = std::vector<BaseLayer*>;

        struct LayerInfo
        {
                QString name;
                bool isVisible = false;
        };

        using LayersSettings = std::vector<LayerInfo>;

    public:
        void setCenterPosition(const QPointF & center);
        MapSettings & getMapSettings();

        QString getOSMDirectoryPath();
        void setOSMDirectoryPath(QString path);

        void addLayer(BaseLayer * layer);

        class OSMLayer * getOSMLayer();
        class GpxLayer * getGpxLayer();

        QPointF getTopLeft();
        QPointF getBottomRight();

        void centerToPoint(const CenterPointStruct & centerPoint);

        const Layers & getLayers() const;

        // interface zo StoreConfigInterface
        void storeConfig(QDomDocument & document, QDomElement & rootElement);
        bool restoreConfig(QDomDocument & document);

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
        enum SelectedAreaState
        {
            Unselecting,
            PrepareForSelectAndDownloadRec,
            PrepareForSelectAndDownloadPoly,
            PrepareForSelectAndDownloadLine,
            PrepareForSelectAndDownloadCircle,
            PrepareForSelectAndFindRec,
            PrepareForSelectAndFindPoly,
            SelectAndDownloadRec,
            SelectAndDownloadPoly,
            SelectAndDownloadLine,
            SelectAndDownloadCircle,
            SelectAndFindRec,
            SelectAndFindPoly
        };

    private:
        void centerToWgs(double lon, double lat);
        QPointF getWgsPointFromPixelsPoint(const QPoint & point);
        LayersSettings generateLayerSettings();
        void restoreLayerSettings(const LayersSettings & settings);
        void createCircleGeometry();

    private slots:
        void startSelectAndDownloadAreaRec();
        void startSelectAndDownloadAreaPoly();
        void startSelectAndDownloadAreaLine();
        void startSelectAndDownloadAreaCircle();
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
        AreaGeometry::CircleGeometry _circle; // z dovodu jednoduchosti kreslenia su suradnice aj radius v pixeloch
};

#endif // PAINTINGWIDGET_H
