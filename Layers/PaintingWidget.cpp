#include "PaintingWidget.h"
#include "OSMLayer.h"
#include "GridLayer.h"
#include "MapContextMenu.h"

#include <iostream>

#include <QDebug>

PaintingWidget::PaintingWidget(QWidget *parent)
    : QWidget(parent)
{
    _mapSettings.widget = this;

    _layers.push_back(LayerInfo(new OSMLayer(_mapSettings), "OSM"));
    _layers.push_back(LayerInfo(new GridLayer(_mapSettings), "Grid"));

    setMouseTracking(true);
}

void PaintingWidget::paintEvent(QPaintEvent *paintEvent)
{
    QWidget::paintEvent(paintEvent);

    for(size_t i = 0; i < _layers.size(); i++)
    {
        if(_layers[i].layer != nullptr)
        {
            _layers[i].layer->paintEvent(paintEvent);
        }
    }
}

void PaintingWidget::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    QWidget::mouseMoveEvent(mouseEvent);

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
        _mapSettings.widget->repaint();
        //std::cout << "mouseMoveEvent delta  " << delta.x() << " " << delta.y() << std::endl;
        //std::cout << "mouseMoveEvent center  " << _mapSettings.worldCenter.x() << " " << _mapSettings.worldCenter.y() << std::endl;
    }
}

void PaintingWidget::mousePressEvent(QMouseEvent *mouseEvent)
{
    if(mouseEvent->buttons() & Qt::MouseButton::LeftButton)
    {
        QWidget * widget = this->parentWidget();
        QWidget * widget2 = widget->parentWidget();

        _mapSettings.movingStartPoint = _mapSettings.widget->mapFrom(widget, QPoint(int(mouseEvent->localPos().x()), int(mouseEvent->localPos().y())));
        _mapSettings.oldCenter = _mapSettings.worldCenter;
        //std::cout << "mousePressEvent " << mouseEvent->localPos().x() << " " << mouseEvent->localPos().y() << std::endl;
    }
    else if(mouseEvent->buttons() & Qt::MouseButton::RightButton)
    {
        if(_contextMenu == nullptr)
        {
            _contextMenu = new MapContextMenu(this);
            QObject::connect(_contextMenu, SIGNAL(downloadArea()), SIGNAL(downloadArea()));
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
        centerAfterZoom(lon, lat);
    }

    _mapSettings.widget->repaint();
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
            setCenterPosition();
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
        centerAfterZoom(lon, lat);
    }

    _mapSettings.widget->repaint();
}

void PaintingWidget::centerAfterZoom(double lon, double lat)
{
    int newPixCenterX = _mapSettings.getPixelForLon(lon);
    int newPixCenterY = _mapSettings.getPixelForLat(lat);

    int worldPixCenterX = _mapSettings.getPixelForLon(0);
    int worldPixCenterY = _mapSettings.getPixelForLat(0);

    int diffPixX = newPixCenterX - worldPixCenterX;
    int diffPixY = newPixCenterY - worldPixCenterY;

    _mapSettings.worldCenter.rx() = (width() / 2) - diffPixX;
    _mapSettings.worldCenter.ry() = (height() / 2) - diffPixY;
}

MapSettings & PaintingWidget::getMapSettings()
{
    return _mapSettings;
}

QPointF PaintingWidget::getCenterPosition()
{
    return QPoint(width() / 2, height() / 2);
}

void PaintingWidget::setCenterPosition()
{
    _mapSettings.worldCenter = getCenterPosition();

    _mapSettings.widget->repaint();
}

void PaintingWidget::setCenterPosition(const QPointF & center)
{
    _mapSettings.worldCenter = center;

    _mapSettings.widget->repaint();
}

void PaintingWidget::setOSMDirectoryPath(QString path)
{
    OSMLayer * osmLayer = getOSMLayer();

    if(osmLayer != nullptr)
    {
        osmLayer->setOSMDirectorypath(path);
    }
}

QString PaintingWidget::getOSMDirectoryPath()
{
    OSMLayer * osmLayer = getOSMLayer();

    if(osmLayer != nullptr)
    {
        return osmLayer->getOSMDirectorypath();
    }

    return QString();
}

OSMLayer * PaintingWidget::getOSMLayer()
{
    for(LayerInfo layerInfo : _layers)
    {
        BaseLayer * layer = layerInfo.layer;

        if(layer != nullptr)
        {
            OSMLayer * osmLayer = dynamic_cast<OSMLayer*>(layer);

            return osmLayer;
        }
    }

    return nullptr;
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
