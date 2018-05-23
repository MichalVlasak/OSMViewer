#include "GpxLayer.h"
#include "../MainWindow.h"

#include <QPainter>

#include <iostream>

GpxLayer::GpxLayer(MapSettings & mapSettings, QWidget * parent)
    : QWidget(parent),
      BaseLayer(mapSettings)
{
}

void GpxLayer::setGpxManager(GpxManager *gpxManager)
{
    _gpxManager = gpxManager;
}

void GpxLayer::paintEvent(QPainter & painter)
{
    if(_gpxManager != nullptr)
    {
        const GpxManager::GpxVector & gpxVector = _gpxManager->getGpxVector();

        for(int id : _currentGpx)
        {
            for(const GpxManager::GpxItem & item : gpxVector)
            {
                if(id == item.fileId)
                {
                    const GpxManager::PointVector & points = item.pointVector;
                    QPolygon polygon;

                    int maxDistance = 5;
                    size_t idx;
                    bool isFinded = false;

                    for(size_t i = 0; i < points.size(); i++)
                    {
                        int pxX = _mapSettings.getPixelForLon(points[i].lon);
                        int pxY = _mapSettings.getPixelForLat(points[i].lat);

                        int newDistance = sqrt(pow(fabs(pxX - _mouseX), 2) + pow(fabs(pxY - _mouseY), 2));

                        if(newDistance < maxDistance)
                        {
                            maxDistance = newDistance;
                            idx = i;
                            isFinded = true;
                        }

                        polygon.push_back(QPoint(pxX, pxY));
                    }

                    painter.setPen(QPen(QColor(255, 0, 0, 255), 3));

                    painter.drawPolygon(polygon);

                    if(isFinded == true)
                    {
                        QPoint point = polygon.at(idx);
                        QPoint topLeft = point;
                        QFontMetrics fontMetrics = painter.fontMetrics();

                        painter.setBrush(QColor(255, 0, 0));
                        painter.drawEllipse(point, 5, 5);

                        QString heightStr = "Height: " + QString::number(item.pointVector[idx].elevation);
                        QString cadention = "Cadention: " + QString::number(item.pointVector[idx].cadention);
                        QString heartRate = "Hearth Rate: " + QString::number(item.pointVector[idx].heartRate);
                        QString temperature = "Temperature: " + QString::number(item.pointVector[idx].temperature);
                        QString time = "Time: ";

                        if(item.pointVector[idx].time.isNull() == false)
                        {
                            QDateTime dateTime = item.pointVector[idx].time.toDateTime();

                            time += dateTime.toString("HH:mm");
                        }

                        int height = fontMetrics.height();
                        int width = fontMetrics.width(heightStr);
                        width = std::max(width, fontMetrics.width(cadention));
                        width = std::max(width, fontMetrics.width(heartRate));
                        width = std::max(width, fontMetrics.width(temperature));
                        width = std::max(width, fontMetrics.width(time));

                        point.rx() = point.rx() + 10;

                        topLeft.rx() = topLeft.rx() + 5;
                        topLeft.ry() = topLeft.ry() - height;

                        QPoint bottomRight = point;

                        bottomRight.rx() = bottomRight.rx() + width + 5;
                        bottomRight.ry() = bottomRight.ry() + 4 * height + height / 2;

                        QRect rect = QRect(topLeft, bottomRight);

                        painter.fillRect(rect, QBrush(QColor(255, 0, 0, 150)));

                        painter.setPen(QColor(0, 0, 0));
                        painter.drawText(point, heightStr);

                        point.ry() = point.ry() + height;
                        painter.drawText(point, cadention);

                        point.ry() = point.ry() + height;
                        painter.drawText(point, heartRate);

                        point.ry() = point.ry() + height;
                        painter.drawText(point, temperature);

                        point.ry() = point.ry() + height;
                        painter.drawText(point, time);
                    }
                }
            }
        }
    }
}

void GpxLayer::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    if(_currentGpx.size() > 0)
    {
        _mouseX = mouseEvent->pos().x();
        _mouseY = mouseEvent->pos().y();

        emit refreshView();
    }
    else
    {
        _mouseX = -1;
        _mouseY = -1;
    }
}

void GpxLayer::setCurrentGpxIndexes(const GpxManager::GpxIdVector &ids)
{
    _currentGpx = ids;

    emit refreshView();
}
