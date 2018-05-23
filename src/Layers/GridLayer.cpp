#include "GridLayer.h"
#include "OSMLayer.h"
#include "../WgsConversion.h"

#include <QPainter>
#include <QWidget>

#include <iostream>
#include <cmath>

#include <QDebug>

GridLayer::GridLayer(MapSettings & mapSettings)
    : BaseLayer(mapSettings)
{
    _gridLevel.push_back(GridLevelInfo(QColor(Qt::red)    , 1.0, 60. * 60. * 10., 0, false));
    _gridLevel.push_back(GridLevelInfo(QColor(Qt::gray)   , 1.0, 60. * 60.      , 1, false));
    _gridLevel.push_back(GridLevelInfo(QColor(Qt::blue)   , 1.0, 60. * 10.      , 2, false));
    _gridLevel.push_back(GridLevelInfo(QColor(Qt::magenta), 1.0, 60.            , 3, false));
    _gridLevel.push_back(GridLevelInfo(QColor(Qt::cyan)   , 1.0, 10.            , 4, false));
    _gridLevel.push_back(GridLevelInfo(QColor(Qt::black)  , 1.0, 1.             , 5, false));
}

void GridLayer::paintEvent(QPainter & painter)
{
    paintGrid(_gridLevel[5], painter);
    paintGrid(_gridLevel[4], painter);
    paintGrid(_gridLevel[3], painter);
    paintGrid(_gridLevel[2], painter);
    paintGrid(_gridLevel[1], painter);
    paintGrid(_gridLevel[0], painter);
}

void GridLayer::paintGrid(GridLevelInfo levelInfo, QPainter & painter)
{
    int wWidth = _mapSettings.widget->width();
    int wHeight = _mapSettings.widget->height();
    double degUnit = levelInfo.gridScale / (60. * 60.);

    if(levelInfo.level > 0)
    {
        double pos1 = _mapSettings.getPixelForLon(degUnit);
        double pos2 = _mapSettings.getPixelForLon(degUnit + (_gridLevel[levelInfo.level - 1].gridScale) / (60 * 60));

        if((pos2 - pos1) < wWidth / 2)
        {
            return;
        }
    }

    QFontMetrics fontMetrics = painter.fontMetrics();

    float degToPix =  OSMLayer::TileSize / _mapSettings.zoom.getCurrentDegreeForTile();
    float offset = (OSMLayer::TileSize / 2.) * pow(2., _mapSettings.zoom.getCurrentZoomLevel());

    painter.setPen(QPen(levelInfo.color, levelInfo.lineWidth));

    // poludniky:
    int labelPosYLatFirst = 0;
    int labelPosYLatSecond = 0;

    double latStartDeg = _mapSettings.getLatForPixel(_mapSettings.windowPixelToMapPixelY(wHeight));
    double latStopDeg = _mapSettings.getLatForPixel(_mapSettings.windowPixelToMapPixelY(0));

    double latUnitStart = latStartDeg * 60 * 60;
    double latUnitStop = latStopDeg * 60 * 60;

    latUnitStart -= fmod(latUnitStart, levelInfo.gridScale);
    latUnitStop -= fmod(latUnitStop, levelInfo.gridScale) - levelInfo.gridScale;

    if(latUnitStart < (DegreeUnit / -4.) || levelInfo.level == 0)
    {
        latUnitStart = DegreeUnit / -4.;
    }

    if(latUnitStop > (DegreeUnit / 4.) || levelInfo.level == 0)
    {
        latUnitStop = DegreeUnit / 4.;
    }

    for(double latUnit = latUnitStart; latUnit <= latUnitStop; latUnit += levelInfo.gridScale)
    {
        double lat = latUnit / levelInfo.gridScale * degUnit;
        int posY = _mapSettings.getPixelForLat(lat);

        if(posY >= (wHeight / 2) && lat < 80.)
        {
            labelPosYLatFirst = posY;
            labelPosYLatSecond = _mapSettings.getPixelForLat(lat + degUnit);
        }
    }

    int posXStart = _mapSettings.worldCenter.x() + (-180. * degToPix);
    int posXStop = _mapSettings.worldCenter.x() + (180. * degToPix);

    posXStart = (posXStart >= 0) ? posXStart : 0;
    posXStop = (posXStop <= wWidth) ? posXStop : wWidth;

    int posYStart = _mapSettings.worldCenter.y() - offset;
    int posYStop = _mapSettings.worldCenter.y() + offset;

    posYStart = (posYStart >= 0) ? posYStart : 0;
    posYStop = (posYStop <= wHeight) ? posYStop : wHeight;

    int labelPosY = labelPosYLatSecond + ((labelPosYLatFirst - labelPosYLatSecond) / 2);
    int lastLabelPosX = _mapSettings.getPixelForLon(-180.);

    double lonStartDeg = _mapSettings.getLonForPixelOld(0);
    double lonStopDeg = _mapSettings.getLonForPixelOld(wWidth);

    double lonUnitStart = lonStartDeg * 60 * 60;
    double lonUnitStop = lonStopDeg * 60 * 60;

    lonUnitStart -= fmod(lonUnitStart, levelInfo.gridScale);
    lonUnitStop -= fmod(lonUnitStop, levelInfo.gridScale) - levelInfo.gridScale;

    if(lonUnitStart < (DegreeUnit / -2.) || levelInfo.level == 0)
    {
        lonUnitStart = DegreeUnit / -2.;
    }

    if(lonUnitStop > (DegreeUnit / 2.) || levelInfo.level == 0)
    {
        lonUnitStop = DegreeUnit / 2.;
    }

    for(double lonUnit = lonUnitStart ; lonUnit <= lonUnitStop; lonUnit += levelInfo.gridScale)
    {
        double lon = lonUnit / levelInfo.gridScale * degUnit;
        int posX = _mapSettings.getPixelForLon(lon);

        if(posX > 0 && posX < wWidth && labelPosY > 0 && labelPosY < wHeight)
        {
            painter.drawLine(posX, posYStart, posX, posYStop);

            QString label = WgsConversion::convertDoubleSecToWgs(lonUnit, WgsConversion::Longitude);
            int labelWidth = fontMetrics.width(label);

            if((lastLabelPosX + 5 + labelWidth) < posX || lon == -180.)
            {
                painter.drawText(QPointF(posX + 5, labelPosY), label);

                lastLabelPosX = posX;
            }
        }
    }

    // rovnobezky:

    // vypocitam si dva poludniky ktore su okolo stredu a na nich budem kreslit
    // hodnotu rovnobezky
    int labelPosXLonFirst = 0;
    int labelPosXLonSecond = 0;

    for(double lonUnit = lonUnitStart ; lonUnit <= lonUnitStop; lonUnit += levelInfo.gridScale)
    {
        double lon = lonUnit / levelInfo.gridScale * degUnit;
        int posX = _mapSettings.getPixelForLon(lon);

        if(posX <= (wWidth / 2))
        {
            labelPosXLonFirst = posX;
            labelPosXLonSecond = _mapSettings.getPixelForLon(lon + degUnit);
        }
    }

    int labelPosX = labelPosXLonFirst + ((labelPosXLonSecond - labelPosXLonFirst) / 2);

    for(double latUnit = latUnitStart; latUnit <= latUnitStop; latUnit += levelInfo.gridScale)
    {
        double lat = latUnit / levelInfo.gridScale * degUnit;
        float posY = _mapSettings.getPixelForLat(lat);

        if(posY > wHeight)
        {
            continue;
        }
        else if(posY < 0)
        {
            break;
        }
        else if(posY >= 0 && posY <= wHeight)
        {
            painter.drawLine(posXStart, posY, posXStop, posY);
            QString label = WgsConversion::convertDoubleSecToWgs(latUnit, WgsConversion::Latitude);
            int labelWidth = fontMetrics.width(label);

            painter.drawText(QPointF(labelPosX - (labelWidth / 2), posY + 13), label);
        }
    }
}
