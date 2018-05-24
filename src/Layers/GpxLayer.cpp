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

        if(gpxVector.size() > 0)
        {
            float maxDistance = 5;
            size_t posIdx;
            int gpxId = -1;
            bool isFinded = false;

            for(int id : _currentGpx)
            {
                for(const GpxManager::GpxItem & item : gpxVector)
                {
                    if(id == item.fileId)
                    {
                        const GpxManager::PointVector & points = item.pointVector;
                        QPolygon polygon;
                        QPoint start;
                        QPoint stop;
                        size_t count = points.size();

                        for(size_t i = 0; i < count; i++)
                        {
                            int pxX = _mapSettings.getPixelForLon(points[i].lon);
                            int pxY = _mapSettings.getPixelForLat(points[i].lat);

                            if(i == 0)
                            {
                                start = QPoint(pxX, pxY);
                            }
                            else if(i == count - 1)
                            {
                                stop = QPoint(pxX, pxY);
                            }

                            float newDistance = sqrt(pow(fabs(pxX - _mouseX), 2.) + pow(fabs(pxY - _mouseY), 2.));

                            if(newDistance < maxDistance)
                            {
                                maxDistance = newDistance;
                                posIdx = i;
                                gpxId = id;;

                                isFinded = true;
                            }

                            polygon.push_back(QPoint(pxX, pxY));
                        }

                        painter.setPen(QPen(QColor(255, 0, 0, 255), 3));

                        painter.drawPolyline(polygon);

                        painter.setBrush(QColor(0, 255, 0));
                        painter.drawEllipse(start, 8, 8);

                        painter.setBrush(QColor(255, 0, 0));
                        painter.drawEllipse(stop, 8, 8);
                    }
                }
            }

            if(isFinded == true)
            {
                for(const GpxManager::GpxItem & item : gpxVector)
                {
                    if(gpxId == item.fileId)
                    {
                        int pxX = _mapSettings.getPixelForLon(item.pointVector[posIdx].lon);
                        int pxY = _mapSettings.getPixelForLat(item.pointVector[posIdx].lat);

                        QPoint point = QPoint(pxX, pxY);
                        QPoint topLeft = point;
                        QFontMetrics fontMetrics = painter.fontMetrics();

                        painter.setBrush(QColor(255, 0, 0));
                        painter.drawEllipse(point, 5, 5);

                        QString heightStr = "Height: " + QString::number(item.pointVector[posIdx].elevation, 'f', 1) + " m";
                        QString cadention = "Cadention: " + QString::number(item.pointVector[posIdx].cadention);
                        QString heartRate = "Hearth Rate: " + QString::number(item.pointVector[posIdx].heartRate) + " bpm";
                        QString temperature = "Temperature: " + QString::number(item.pointVector[posIdx].temperature) + " °C";
                        QString dayTime = "Day Time: ";
                        QString tripTime = "Trip Time: ";

                        if(item.pointVector[posIdx].time.isNull() == false)
                        {
                            QDateTime dateTime = item.pointVector[posIdx].time.toDateTime();

                            dayTime += dateTime.toString("HH:mm");
                        }

                        if(item.pointVector[posIdx].time.isNull() == false && item.time.isNull() == false)
                        {
                            QDateTime dateTime = item.pointVector[posIdx].time.toDateTime();
                            QDateTime startDateTime = item.time.toDateTime();

                            time_t time = dateTime.toTime_t() - startDateTime.toTime_t();

                            int hour = int(time / 3600);
                            time -= hour * 3600;
                            int min = int(time / 60);
                            time -= min * 60;
                            int sec = time;

                            tripTime += QString("%1:%2:%3").arg(hour).arg(min, 2,'f', 0,'0').arg(sec, 2,'f', 0,'0');
                        }

                        int height = fontMetrics.height();
                        int width = fontMetrics.width(heightStr);
                        width = std::max(width, fontMetrics.width(cadention));
                        width = std::max(width, fontMetrics.width(heartRate));
                        width = std::max(width, fontMetrics.width(temperature));
                        width = std::max(width, fontMetrics.width(dayTime));
                        width = std::max(width, fontMetrics.width(tripTime));

                        point.rx() = point.rx() + 10;

                        topLeft.rx() = topLeft.rx() + 5;
                        topLeft.ry() = topLeft.ry() - height;

                        QPoint bottomRight = point;

                        bottomRight.rx() = bottomRight.rx() + width + 5;
                        bottomRight.ry() = bottomRight.ry() + 5 * height + height / 2;

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
                        painter.drawText(point, dayTime);

                        point.ry() = point.ry() + height;
                        painter.drawText(point, tripTime);

                        break;
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
