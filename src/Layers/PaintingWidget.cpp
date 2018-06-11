#include "PaintingWidget.h"
#include "OSMLayer.h"
#include "GridLayer.h"
#include "GpxLayer.h"
#include "src/MapContextMenu.h"
#include "src/CenterPointsManager.h"
#include "src/MainWindow.h"

#include <iostream>

#include <QDebug>
#include <QPainter>
#include <QTimer>

PaintingWidget::PaintingWidget(QWidget *parent)
    : QWidget(parent)
{
    _mapSettings.widget = this;

    _selectedAreaState = Unselecting;

    _startPointSelectArea = QPoint(0, 0);
    _endPointSelectArea = QPoint(0, 0);

    _osmLayer = new OSMLayer(_mapSettings);
    _gpxLayer = new GpxLayer(_mapSettings, this);

    _layers.push_back(LayerInfo(_osmLayer, "OSM"));
    _layers.push_back(LayerInfo(_gpxLayer, "GPX"));
    _layers.push_back(LayerInfo(new GridLayer(_mapSettings), "Grid"));

    setMouseTracking(true);

    QObject::connect(_gpxLayer, SIGNAL(refreshView()), SLOT(repaint()));
}

void PaintingWidget::addLayer(BaseLayer *layer, QString layerName)
{
    _layers.push_back(LayerInfo(layer, layerName));
}

void PaintingWidget::paintEvent(QPaintEvent *paintEvent)
{
    QWidget::paintEvent(paintEvent);

    if(paintEvent->type() == QPaintEvent::Paint)
    {
        QPainter painter(_mapSettings.widget);

        for(size_t i = 0; i < _layers.size(); i++)
        {
            if(_layers[i].layer != nullptr)
            {
                _layers[i].layer->paintEvent(painter);
            }
        }

        if(_selectedAreaState == Selecting)
        {
            QRect rect = QRect(_startPointSelectArea, _endPointSelectArea);

            painter.drawRect(rect);
        }
    }
}

void PaintingWidget::startSelectArea()
{
    _selectedAreaState = PrepareForSelecting;
}

void PaintingWidget::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    QWidget::mouseMoveEvent(mouseEvent);

    for(size_t i = 0; i < _layers.size(); i++)
    {
        if(_layers[i].layer != nullptr)
        {
            _layers[i].layer->mouseMoveEvent(mouseEvent);
        }
    }

    if(_selectedAreaState == Selecting)
    {
        QPoint point = QPoint(int(mouseEvent->localPos().x()), int(mouseEvent->localPos().y()));

        _endPointSelectArea = point;

        QTimer::singleShot(1, this, SLOT(repaint()));
    }
    else
    {
        int pixX = mouseEvent->pos().x();
        int pixY = mouseEvent->pos().y();
        //double lon = _mapSettings.getLonForPixel(_mapSettings.windowPixelToMapPixelX(pixX));
        double lon = _mapSettings.getLonForPixelOld(pixX);
        double lat = _mapSettings.getLatForPixel(_mapSettings.windowPixelToMapPixelY(pixY));

        emit mouseCursorWgsChanged(lat, lon);

        //qDebug() << mouseEvent->buttons() << " " << mouseEvent->pos() << " " << lon << " " << lat;

        if(mouseEvent->buttons() & Qt::MouseButton::LeftButton)
        {
            QPointF currentPoint = mouseEvent->localPos();
            QPointF delta = currentPoint - _mapSettings.movingStartPoint;
            _mapSettings.worldCenter = _mapSettings.oldCenter + delta;

            QTimer::singleShot(1, _mapSettings.widget, SLOT(repaint()));
            //std::cout << "mouseMoveEvent delta  " << delta.x() << " " << delta.y() << std::endl;
            //std::cout << "mouseMoveEvent center  " << _mapSettings.worldCenter.x() << " " << _mapSettings.worldCenter.y() << std::endl;
        }
    }
}

void PaintingWidget::mousePressEvent(QMouseEvent *mouseEvent)
{
    if(mouseEvent->buttons() & Qt::MouseButton::LeftButton)
    {
        QPoint point = QPoint(int(mouseEvent->localPos().x()), int(mouseEvent->localPos().y()));

        if(_selectedAreaState == PrepareForSelecting)
        {
            _selectedAreaState = Selecting;

            _startPointSelectArea = point;
            _endPointSelectArea = point;

            QTimer::singleShot(1, this, SLOT(repaint()));
        }
        else if(_selectedAreaState == Selecting)
        {
            _selectedAreaState = Unselecting;
            _endPointSelectArea = point;

            double lon = _mapSettings.getLonForPixelOld(_startPointSelectArea.x());
            double lat = _mapSettings.getLatForPixel(_mapSettings.windowPixelToMapPixelY(_startPointSelectArea.y()));

            QPointF topLeft(lon, lat);

            lon = _mapSettings.getLonForPixelOld(_endPointSelectArea.x());
            lat = _mapSettings.getLatForPixel(_mapSettings.windowPixelToMapPixelY(_endPointSelectArea.y()));

            QPointF bottomleft(lon, lat);

            emit downloadSelectedArea(topLeft, bottomleft);

            QTimer::singleShot(1, this, SLOT(repaint()));
        }
        else
        {
            QWidget * widget = this->parentWidget();

            _mapSettings.movingStartPoint = _mapSettings.widget->mapFrom(widget, point);
            _mapSettings.oldCenter = _mapSettings.worldCenter;
            //std::cout << "mousePressEvent " << mouseEvent->localPos().x() << " " << mouseEvent->localPos().y() << std::endl;
        }
    }
    else if(mouseEvent->buttons() & Qt::MouseButton::RightButton)
    {
        if(_contextMenu == nullptr)
        {
            _contextMenu = new MapContextMenu(this);
            QObject::connect(_contextMenu, SIGNAL(downloadArea()), SIGNAL(downloadArea()));
            QObject::connect(_contextMenu, SIGNAL(selectAndDownloadArea()), SLOT(startSelectArea()));
            QObject::connect(_contextMenu, SIGNAL(centerMap(QPoint)), SLOT(centerMapToPixels(QPoint)));
        }

        if(_contextMenu != nullptr)
        {
            _contextMenu->show(mouseEvent->localPos());
        }
    }
    else
    {
        QWidget::mousePressEvent(mouseEvent);
    }
}

void PaintingWidget::wheelEvent(QWheelEvent *wheelEvent)
{
    QWidget::wheelEvent(wheelEvent);

    int pixX = wheelEvent->pos().x();
    int pixY = wheelEvent->pos().y();
    //double lon = _mapSettings.getLonForPixel(_mapSettings.windowPixelToMapPixelX(pixX));
    double lon = _mapSettings.getLonForPixelOld(pixX);
    double lat = _mapSettings.getLatForPixel(_mapSettings.windowPixelToMapPixelY(pixY));

    int oldZoom = _mapSettings.zoom.getCurrentZoomLevel();

    if(wheelEvent->delta() > 0)
    {
        _mapSettings.zoom.zoomIncrement();
    }
    else
    {
        _mapSettings.zoom.zoomDecrement();
    }

    int newZoom = _mapSettings.zoom.getCurrentZoomLevel();

    if(newZoom != oldZoom)
    {
        centerToWgs(lon, lat);
    }

    QTimer::singleShot(1, _mapSettings.widget, SLOT(repaint()));
}

void PaintingWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    QWidget::keyPressEvent(keyEvent);

    switch (keyEvent->key())
    {
        case Qt::Key_Left:
            _mapSettings.worldCenter.rx() += 10.;
            break;

        case Qt::Key_Right:
            _mapSettings.worldCenter.rx() -= 10.;
            break;

        case Qt::Key_Up:
            _mapSettings.worldCenter.ry() += 10.;
            break;

        case Qt::Key_Down:
            _mapSettings.worldCenter.ry() -= 10.;
            break;

        case Qt::Key_Home:
            {
                CenterPointsManager * pointsManager = MainWindow::getInstance()->getCenterPointsManager();

                if(pointsManager != nullptr)
                {
                    centerToPoint(pointsManager->getHomeCenterPoint());
                }
            }
            break;

        case Qt::Key_Escape:
            _selectedAreaState = Unselecting;
            break;

        default:
            break;
    }

    //double lon = _mapSettings.getLonForPixel(_mapSettings.windowPixelToMapPixelX(width() / 2.));
    double lon = _mapSettings.getLonForPixelOld(width() / 2.);
    double lat = _mapSettings.getLatForPixel(_mapSettings.windowPixelToMapPixelY(height() / 2));

    int oldZoom = _mapSettings.zoom.getCurrentZoomLevel();
    _mapSettings.zoom.keyPressEvent(keyEvent);

    int newZoom = _mapSettings.zoom.getCurrentZoomLevel();

    if(newZoom != oldZoom)
    {
        centerToWgs(lon, lat);
    }

    QTimer::singleShot(1, _mapSettings.widget, SLOT(repaint()));
}

void PaintingWidget::centerToPoint(const CenterPointStruct & centerPoint)
{
    if(centerPoint.level != CenterPointStruct::UndefinedLevel)
    {
        _mapSettings.zoom.setCurrentZoomLevel(centerPoint.level);
    }

    centerToWgs(centerPoint.position.x(), centerPoint.position.y());
}

void PaintingWidget::centerToWgs(double lon, double lat)
{
    int newPixCenterX = _mapSettings.getPixelForLon(lon);
    int newPixCenterY = _mapSettings.getPixelForLat(lat);

    int worldPixCenterX = _mapSettings.getPixelForLon(0);
    int worldPixCenterY = _mapSettings.getPixelForLat(0);

    int diffPixX = newPixCenterX - worldPixCenterX;
    int diffPixY = newPixCenterY - worldPixCenterY;

    _mapSettings.worldCenter.rx() = (width() / 2) - diffPixX;
    _mapSettings.worldCenter.ry() = (height() / 2) - diffPixY;

    QTimer::singleShot(1, _mapSettings.widget, SLOT(repaint()));
}

MapSettings & PaintingWidget::getMapSettings()
{
    return _mapSettings;
}

void PaintingWidget::setCenterPosition(const QPointF & center)
{
    _mapSettings.worldCenter = center;

    QTimer::singleShot(1, _mapSettings.widget, SLOT(repaint()));
}

void PaintingWidget::setOSMDirectoryPath(QString path)
{
    if(_osmLayer != nullptr)
    {
        _osmLayer->setOSMDirectorypath(path);
    }
}

QString PaintingWidget::getOSMDirectoryPath()
{
    if(_osmLayer != nullptr)
    {
        return _osmLayer->getOSMDirectorypath();
    }

    return QString();
}

OSMLayer * PaintingWidget:: getOSMLayer()
{
    return _osmLayer;
}

QPointF PaintingWidget::getBottomRight()
{
    double lon = _mapSettings.getLonForPixelOld(width());
    double lat = _mapSettings.getLatForPixel(_mapSettings.windowPixelToMapPixelY(height()));

    return QPointF(lon, lat);
}

QPointF PaintingWidget::getTopLeft()
{
    double lon = _mapSettings.getLonForPixelOld(0);
    double lat = _mapSettings.getLatForPixel(_mapSettings.windowPixelToMapPixelY(0));

    return QPointF(lon, lat);
}

void PaintingWidget::centerMapToPixels(QPoint pos)
{
    int pixX = pos.x();
    int pixY = pos.y();

    double lon = _mapSettings.getLonForPixelOld(pixX);
    double lat = _mapSettings.getLatForPixel(_mapSettings.windowPixelToMapPixelY(pixY));

    centerToWgs(lon, lat);
}

GpxLayer * PaintingWidget::getGpxLayer()
{
    return _gpxLayer;
}
