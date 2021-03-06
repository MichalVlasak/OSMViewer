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
    
    _startPointSelectRectangle = QPoint(0, 0);
    _endPointSelectRectangle = QPoint(0, 0);
    _selectedPolygon.clear();
    _selectedLine.clear();
    _selectGeometry.geometry.clear();
    _selectGeometry.geometryType = AreaGeometry::Type::Undefined;
    
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

        painter.setPen(QPen(QColor(255, 0, 0), 1));
        painter.setBrush(QColor(0, 0, 0, 0));
        
        if(_selectedAreaState == SelectAndDownloadRec || _selectedAreaState == SelectAndFindRec)
        {
            QRect rect = QRect(_startPointSelectRectangle, _endPointSelectRectangle);
            
            painter.drawRect(rect);
        }
        else if(_selectedAreaState == SelectAndDownloadPoly || _selectedAreaState == SelectAndFindPoly)
        {
            QPolygon pxPolygon;
            
            for(const QPointF & point : _selectedPolygon)
            {
                pxPolygon.push_back(QPoint(int(_mapSettings.getPixelForLon(point.x())), int(_mapSettings.getPixelForLat(point.y()))));
            }
            
            painter.drawPolygon(pxPolygon);
        }
        else if(_selectedAreaState == SelectAndDownloadLine)
        {
            QPolygon pxPolygon;
            
            for(const QPointF & point : _selectedLine)
            {
                pxPolygon.push_back(QPoint(int(_mapSettings.getPixelForLon(point.x())), int(_mapSettings.getPixelForLat(point.y()))));
            }
            
            painter.drawPolyline(pxPolygon);
        }
    }
}

void PaintingWidget::startSelectAndDownloadAreaRec()
{
    _selectedAreaState = PrepareForSelectAndDownloadRec;
}

void PaintingWidget::startSelectAndDownloadAreaPoly()
{
    _selectedAreaState = PrepareForSelectAndDownloadPoly;
}

void PaintingWidget::startSelectAndDownloadAreaLine()
{
    _selectedAreaState = PrepareForSelectAndDownloadLine;
}

void PaintingWidget::startSelectAndFindAreaPoly()
{
    _selectedAreaState = PrepareForSelectAndFindPoly;
}

void PaintingWidget::startSelectAndFindAreaRec()
{
    _selectedAreaState = PrepareForSelectAndFindRec;
}

QPointF PaintingWidget::getWgsPointFromPixelsPoint(const QPoint &point)
{
    double lon = _mapSettings.getLonForPixelOld(point.x());
    double lat = _mapSettings.getLatForPixel(_mapSettings.windowPixelToMapPixelY(point.y()));
    
    return QPointF(lon, lat);
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
    
    QPoint point = QPoint(int(mouseEvent->localPos().x()), int(mouseEvent->localPos().y()));
    
    if(_selectedAreaState == SelectAndDownloadRec || _selectedAreaState == SelectAndFindRec)
    {
        _endPointSelectRectangle = point;
    }
    else if(_selectedAreaState == SelectAndDownloadPoly || _selectedAreaState == SelectAndFindPoly)
    {
        if(_selectedPolygon.size() > 1)
        {
            _selectedPolygon.erase(_selectedPolygon.end() - 1);
        }
        
        _selectedPolygon.push_back(getWgsPointFromPixelsPoint(point));
    }
    else if(_selectedAreaState == SelectAndDownloadLine)
    {
        if(_selectedLine.size() > 1)
        {
            _selectedLine.erase(_selectedLine.end() - 1);
        }
        
        _selectedLine.push_back(getWgsPointFromPixelsPoint(point));
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
            
            //std::cout << "mouseMoveEvent delta  " << delta.x() << " " << delta.y() << std::endl;
            //std::cout << "mouseMoveEvent center  " << _mapSettings.worldCenter.x() << " " << _mapSettings.worldCenter.y() << std::endl;
        }
    }
    
    QTimer::singleShot(1, this, SLOT(repaint()));
}

void PaintingWidget::mousePressEvent(QMouseEvent *mouseEvent)
{
    if(mouseEvent->buttons() & Qt::MouseButton::LeftButton)
    {
        QPoint point = QPoint(int(mouseEvent->localPos().x()), int(mouseEvent->localPos().y()));
        
        if(_selectedAreaState == PrepareForSelectAndDownloadRec)
        {
            _selectedAreaState = SelectAndDownloadRec;
            
            _startPointSelectRectangle = point;
            _endPointSelectRectangle = point;
        }
        else if(_selectedAreaState == PrepareForSelectAndDownloadPoly)
        {
            _selectedAreaState = SelectAndDownloadPoly;
            
            _selectedPolygon.clear();
            _selectedPolygon.push_back(getWgsPointFromPixelsPoint(point));
        }
        else if(_selectedAreaState == PrepareForSelectAndDownloadLine)
        {
            _selectedAreaState = SelectAndDownloadLine;
            
            _selectedLine.clear();
            _selectedLine.push_back(getWgsPointFromPixelsPoint(point));
        }
        else if(_selectedAreaState == PrepareForSelectAndFindRec)
        {
            _selectedAreaState = SelectAndFindRec;

            _startPointSelectRectangle = point;
            _endPointSelectRectangle = point;
        }
        else if(_selectedAreaState == PrepareForSelectAndFindPoly)
        {
            _selectedAreaState = SelectAndFindPoly;

            _selectedPolygon.clear();
            _selectedPolygon.push_back(getWgsPointFromPixelsPoint(point));
        }
        else if(_selectedAreaState == SelectAndDownloadRec)
        {
            _selectedAreaState = Unselecting;
            _endPointSelectRectangle = point;
            
            QPointF startPoint(getWgsPointFromPixelsPoint(_startPointSelectRectangle));
            QPointF endPoint(getWgsPointFromPixelsPoint(_endPointSelectRectangle));
            
            if(startPoint.y() < endPoint.y())
            {
                std::swap(startPoint.ry(), endPoint.ry());
            }
            
            if(startPoint.x() > endPoint.x())
            {
                std::swap(startPoint.rx(), endPoint.rx());
            }
            
            _selectGeometry.geometryType = AreaGeometry::Type::Rectangle;
            _selectGeometry.geometry = QRectF(startPoint, endPoint);
            
            emit downloadSelectedArea(_selectGeometry);
        }
        else if(_selectedAreaState == SelectAndDownloadPoly || _selectedAreaState == SelectAndFindPoly)
        {
            _selectedPolygon.push_back(getWgsPointFromPixelsPoint(point));
        }
        else if(_selectedAreaState == SelectAndDownloadLine)
        {
            _selectedLine.push_back(getWgsPointFromPixelsPoint(point));
        }
        else if(_selectedAreaState == SelectAndFindRec)
        {
            _selectedAreaState = Unselecting;
            _endPointSelectRectangle = point;

            QPointF startPoint(getWgsPointFromPixelsPoint(_startPointSelectRectangle));
            QPointF endPoint(getWgsPointFromPixelsPoint(_endPointSelectRectangle));

            if(startPoint.y() < endPoint.y())
            {
                std::swap(startPoint.ry(), endPoint.ry());
            }

            if(startPoint.x() > endPoint.x())
            {
                std::swap(startPoint.rx(), endPoint.rx());
            }

            _selectGeometry.geometryType = AreaGeometry::Type::Rectangle;
            _selectGeometry.geometry = QRectF(startPoint, endPoint);

            emit findInSelectedArea(_selectGeometry);
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
        if(_selectedAreaState == SelectAndDownloadPoly || _selectedAreaState == SelectAndFindPoly)
        {
            if(_selectedPolygon.size() > 1)
            {
                _selectedPolygon.erase(_selectedPolygon.end() - 2);
            }
        }
        if(_selectedAreaState == SelectAndDownloadLine)
        {
            if(_selectedLine.size() > 1)
            {
                _selectedLine.erase(_selectedLine.end() - 2);
            }
        }
        else
        {
            if(_contextMenu == nullptr)
            {
                _contextMenu = new MapContextMenu(this);
                
                QObject::connect(_contextMenu, SIGNAL(downloadArea()), SLOT(downloadViewedAreaSlot()));
                QObject::connect(_contextMenu, SIGNAL(selectAndDownloadAreaRec()), SLOT(startSelectAndDownloadAreaRec()));
                QObject::connect(_contextMenu, SIGNAL(selectAndDownloadAreaPoly()), SLOT(startSelectAndDownloadAreaPoly()));
                QObject::connect(_contextMenu, SIGNAL(selectAndDownloadAreaLine()), SLOT(startSelectAndDownloadAreaLine()));
                QObject::connect(_contextMenu, SIGNAL(selectAndFindAreaPoly()), SLOT(startSelectAndFindAreaPoly()));
                QObject::connect(_contextMenu, SIGNAL(selectAndFindAreaRec()), SLOT(startSelectAndFindAreaRec()));
                QObject::connect(_contextMenu, SIGNAL(centerMap(QPoint)), SLOT(centerMapToPixels(QPoint)));
            }
            
            if(_contextMenu != nullptr)
            {
                _contextMenu->show(mouseEvent->localPos());
            }
        }
    }
    else
    {
        QWidget::mousePressEvent(mouseEvent);
    }
    
    QTimer::singleShot(1, this, SLOT(repaint()));
}

void PaintingWidget::downloadViewedAreaSlot()
{
    AreaGeometry geometry;
    
    geometry.geometryType = AreaGeometry::Type::Rectangle;
    geometry.geometry = QRectF(getTopLeft(), getBottomRight());
    
    emit downloadSelectedArea(geometry);
    
}

void PaintingWidget::mouseDoubleClickEvent(QMouseEvent *mouseEvent)
{
    if(mouseEvent->buttons() & Qt::MouseButton::LeftButton)
    {
        if(_selectedAreaState == SelectAndDownloadPoly)
        {
            if(_selectedPolygon.size() > 1)
            {
                _selectedPolygon.erase(_selectedPolygon.end() - 1);
                _selectedPolygon.push_back(_selectedPolygon.at(0));
            }
            
            AreaGeometry geometry;
            
            geometry.geometryType = AreaGeometry::Type::Polygon;
            geometry.geometry = _selectedPolygon;
            
            emit downloadSelectedArea(geometry);
            
            _selectedAreaState = Unselecting;
            _selectedPolygon.clear();
        }
        else if(_selectedAreaState == SelectAndDownloadLine)
        {
            if(_selectedLine.size() > 1)
            {
                _selectedLine.erase(_selectedLine.end() - 1);
            }
            
            AreaGeometry geometry;
            AreaGeometry::LineBufferGeometry lineGeom;
            
            lineGeom.bufferWidth = 0;
            lineGeom.line = _selectedLine;
            
            geometry.geometryType = AreaGeometry::Type::Line;
            geometry.geometry = QVariant::fromValue(lineGeom);
            
            emit downloadSelectedArea(geometry);
            
            _selectedAreaState = Unselecting;
            _selectedLine.clear();
        }
        else if(_selectedAreaState == SelectAndFindPoly)
        {
            if(_selectedPolygon.size() > 1)
            {
                _selectedPolygon.erase(_selectedPolygon.end() - 1);
                _selectedPolygon.push_back(_selectedPolygon.at(0));
            }

            AreaGeometry geometry;

            geometry.geometryType = AreaGeometry::Type::Polygon;
            geometry.geometry = _selectedPolygon;

            _selectedAreaState = Unselecting;
            _selectedPolygon.clear();

            emit findInSelectedArea(geometry);
        }
    }
    else
    {
        QWidget::mouseDoubleClickEvent(mouseEvent);
    }
    
    
    QTimer::singleShot(1, this, SLOT(repaint()));
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
