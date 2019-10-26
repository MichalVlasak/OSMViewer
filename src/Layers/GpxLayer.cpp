#include "GpxLayer.h"
#include "../MainWindow.h"

#include <QPainter>

#include <iostream>

GpxLayer::GpxLayer(const QString & layerName, MapSettings & mapSettings, QWidget * parent)
    : QWidget(parent),
      BaseLayer(layerName, mapSettings)
{
}

void GpxLayer::setGpxManager(GpxManagerPtr gpxManager)
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
                            QPoint point = getPixelPoint(points[i].lat, points[i].lon);

                            if(i == 0)
                            {
                                start = point;
                            }
                            else if(i == count - 1)
                            {
                                stop = point;
                            }

                            float newDistance = sqrt(pow(fabs(point.rx() - _mouseX), 2.) + pow(fabs(point.ry() - _mouseY), 2.));

                            if(newDistance < maxDistance)
                            {
                                maxDistance = newDistance;
                                posIdx = i;
                                gpxId = id;;

                                isFinded = true;
                            }

                            polygon.push_back(point);
                        }

                        painter.setPen(QPen(QColor(255, 0, 0), 3));

                        painter.drawPolyline(polygon);

                        painter.setBrush(QColor(0, 255, 0));
                        painter.drawEllipse(start, 8, 8);

                        painter.setBrush(QColor(255, 0, 0));
                        painter.drawEllipse(stop, 8, 8);

                        if(item.showBiggestElevetion == true && item.biggestElevetionIdx != GpxManager::ErrorId)
                        {
                            paintPoint(painter, item, item.biggestElevetionIdx, tr("Max Elevation"));
                        }

                        if(item.showBiggestCadention == true && item.biggestCadentionIdx != GpxManager::ErrorId)
                        {
                            paintPoint(painter, item, item.biggestCadentionIdx, tr("Max Cadention"));
                        }

                        if(item.showBiggestTemperature == true && item.biggestTemperatureIdx != GpxManager::ErrorId)
                        {
                            paintPoint(painter, item, item.biggestTemperatureIdx, tr("Max Temperature"));
                        }

                        if(item.showBiggestHeartRate == true && item.biggestHeartRateIdx != GpxManager::ErrorId)
                        {
                            paintPoint(painter, item, item.biggestHeartRateIdx, tr("Max Heart Rate"));
                        }
                    }
                }
            }

            for(const GpxManager::GpxItem & item : gpxVector)
            {
                if(isFinded == true)
                {
                    if(gpxId == item.fileId)
                    {
                        emit changeSelectedPoint(gpxId, posIdx);

                        paintPoint(painter, item, posIdx);

                        _selectedPoints[gpxId] = posIdx;
                    }
                }

                if(isFinded == false || (isFinded == true && gpxId != item.fileId))
                {
                    SelectedPointsMap::iterator it = _selectedPoints.find(item.fileId);

                    if(it != _selectedPoints.end())
                    {
                        size_t idx = it->second;

                        paintPoint(painter, item, idx);
                    }
                }
            }
        }
    }
}

void GpxLayer::paintPoint(QPainter &painter, const GpxManager::GpxItem & item, size_t posIdx, const QString & title)
{
    QPoint point = getPixelPoint(item.pointVector[posIdx].lat, item.pointVector[posIdx].lon);
    QPoint topLeft = point;
    QFontMetrics fontMetrics = painter.fontMetrics();

    int pxX = point.rx();
    int pxY = point.ry();

    painter.setPen(QPen(QColor(255, 0, 0), 1));
    painter.setBrush(QColor(255, 0, 0));
    //painter.drawEllipse(point, 5, 5);

    QPolygon polygon;

    polygon.push_back(point);
    polygon.push_back(QPoint(pxX - 7, pxY - 20));
    polygon.push_back(QPoint(pxX + 7, pxY - 20));

    painter.drawPolygon(polygon);
    painter.drawChord(pxX - 8, pxY - 25, 16, 25, 30 * 16, 120 * 16);

    QString heightStr = "Height: ";
    QString heightValueStr = "--";

    if(item.pointVector[posIdx].elevation.isNull() == false && item.pointVector[posIdx].elevation.canConvert<double>() == true)
    {
        bool isOk = false;

        double elevation = item.pointVector[posIdx].elevation.toDouble(&isOk);

        if(isOk == true)
        {
            heightValueStr = QString::number(elevation, 'f', 1) + " m";
        }
    }

    heightStr += heightValueStr;

    QString cadentionStr = "Cadention: ";
    QString cadentionValueStr = "--";

    if(item.pointVector[posIdx].cadention.isNull() == false && item.pointVector[posIdx].cadention.canConvert<int>() == true)
    {
        bool isOk = false;

        double cadention = item.pointVector[posIdx].cadention.toInt(&isOk);

        if(isOk == true)
        {
            cadentionValueStr = QString::number(cadention);
        }
    }

    cadentionStr += cadentionValueStr;

    QString heartRateStr = "Hearth Rate: ";
    QString heartRateValueStr = "--";

    if(item.pointVector[posIdx].heartRate.isNull() == false && item.pointVector[posIdx].heartRate.canConvert<int>() == true)
    {
        bool isOk = false;

        double heartRate = item.pointVector[posIdx].heartRate.toDouble(&isOk);

        if(isOk == true)
        {
            heartRateValueStr = QString::number(heartRate) + " bpm";
        }
    }

    heartRateStr += heartRateValueStr;

    QString temperatureStr = "Temperature: ";
    QString temperatureValueStr = "--";

    if(item.pointVector[posIdx].temperature.isNull() == false && item.pointVector[posIdx].temperature.canConvert<float>() == true)
    {
        bool isOk = false;

        double temperature = item.pointVector[posIdx].temperature.toFloat(&isOk);

        if(isOk == true)
        {
            temperatureValueStr = QString::number(temperature) + " °C";
        }
    }

    temperatureStr += temperatureValueStr;

    QString dayTime = "Day Time: ";
    QString tripTime = "Trip Time: ";

    if(item.pointVector[posIdx].time.isNull() == false)
    {
        QDateTime dateTime = item.pointVector[posIdx].time.toDateTime();

        dayTime += dateTime.toString("HH:mm");
    }

    if(item.pointVector[posIdx].time.isNull() == false && item.startTime.isNull() == false)
    {
        QDateTime dateTime = item.pointVector[posIdx].time.toDateTime();
        QDateTime startDateTime = item.startTime.toDateTime();

        time_t time = dateTime.toTime_t() - startDateTime.toTime_t();

        int hour = int(time / 3600);
        time -= hour * 3600;
        int min = int(time / 60);
        time -= min * 60;
        int sec = time;

        tripTime += QString("%1:%2:%3").arg(hour).arg(min, 2,'f', 0,'0').arg(sec, 2,'f', 0,'0');
    }

    int height = fontMetrics.height();
    int titleHeight = fontMetrics.height();
    int width = fontMetrics.width(heightStr);
    width = std::max(width, fontMetrics.width(cadentionStr));
    width = std::max(width, fontMetrics.width(heartRateStr));
    width = std::max(width, fontMetrics.width(temperatureStr));
    width = std::max(width, fontMetrics.width(dayTime));
    width = std::max(width, fontMetrics.width(tripTime));

    QFont origFont = painter.font();
    QFont titleFont = origFont;

    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 2);

    if(title.isEmpty() == false)
    {
        painter.setFont(titleFont);

        QFontMetrics titleFontMetric = painter.fontMetrics();

        width = std::max(width, titleFontMetric.width(title));
        titleHeight = std::max(titleHeight, titleFontMetric.height());

        painter.setFont(origFont);
    }

    point.rx() = point.rx() + 15;

    topLeft.rx() = topLeft.rx() + 10;
    topLeft.ry() = topLeft.ry() - titleHeight;

    QPoint bottomRight = point;

    bottomRight.rx() = bottomRight.rx() + width + 5;
    bottomRight.ry() = bottomRight.ry() + ((title.isEmpty() == false) ? 6 : 5) * height + height / 2;

    QRect rect = QRect(topLeft, bottomRight);

    painter.fillRect(rect, QBrush(QColor(255, 0, 0, 150)));

    painter.setPen(QColor(0, 0, 0));

    if(title.isEmpty() == false)
    {
        painter.setFont(titleFont);
        painter.drawText(point, title);

        painter.setFont(origFont);

        point.ry() = point.ry() + height;
    }

    painter.drawText(point, heightStr);

    point.ry() = point.ry() + height;
    painter.drawText(point, cadentionStr);

    point.ry() = point.ry() + height;
    painter.drawText(point, heartRateStr);

    point.ry() = point.ry() + height;
    painter.drawText(point, temperatureStr);

    point.ry() = point.ry() + height;
    painter.drawText(point, dayTime);

    point.ry() = point.ry() + height;
    painter.drawText(point, tripTime);
}

QPoint GpxLayer::getPixelPoint(double lat, double lon)
{
    return QPoint(int(_mapSettings.getPixelForLon(lon)), int(_mapSettings.getPixelForLat(lat)));
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

void GpxLayer::highlightSelectedPoint(int gpxId, size_t idx)
{
    _selectedPoints[gpxId] = idx;

    emit refreshView();
}

void GpxLayer::removeSelectedPoint(int gpxId)
{
    SelectedPointsMap::iterator it = _selectedPoints.find(gpxId);

    if(it != _selectedPoints.end())
    {
        _selectedPoints.erase(it);

        emit refreshView();
    }
}
