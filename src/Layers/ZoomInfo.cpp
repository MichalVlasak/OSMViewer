#include "ZoomInfo.h"

#include <iostream>
#include <cmath>

ZoomInfo::ZoomInfo()
{
    _zoomScale.push_back(ZoomStruct(500000000, 156412.000, 360. / pow(2., 0 )));
    _zoomScale.push_back(ZoomStruct(250000000,  78206.000, 360. / pow(2., 1 )));
    _zoomScale.push_back(ZoomStruct(150000000,  39103.000, 360. / pow(2., 2 )));
    _zoomScale.push_back(ZoomStruct(70000000 ,  19551.000, 360. / pow(2., 3 )));
    _zoomScale.push_back(ZoomStruct(35000000 ,   9776.000, 360. / pow(2., 4 )));
    _zoomScale.push_back(ZoomStruct(15000000 ,   4888.000, 360. / pow(2., 5 )));
    _zoomScale.push_back(ZoomStruct(10000000 ,   2444.000, 360. / pow(2., 6 )));
    _zoomScale.push_back(ZoomStruct(4000000  ,   1222.000, 360. / pow(2., 7 )));
    _zoomScale.push_back(ZoomStruct(2000000  ,    610.984, 360. / pow(2., 8 )));
    _zoomScale.push_back(ZoomStruct(1000000  ,    305.492, 360. / pow(2., 9 )));
    _zoomScale.push_back(ZoomStruct(500000   ,    152.746, 360. / pow(2., 10)));
    _zoomScale.push_back(ZoomStruct(250000   ,     76.373, 360. / pow(2., 11)));
    _zoomScale.push_back(ZoomStruct(150000   ,     38.187, 360. / pow(2., 12)));
    _zoomScale.push_back(ZoomStruct(70000    ,     19.093, 360. / pow(2., 13)));
    _zoomScale.push_back(ZoomStruct(35000    ,      9.547, 360. / pow(2., 14)));
    _zoomScale.push_back(ZoomStruct(15000    ,      4.773, 360. / pow(2., 15)));
    _zoomScale.push_back(ZoomStruct(8000     ,      2.387, 360. / pow(2., 16)));
    _zoomScale.push_back(ZoomStruct(4000     ,      1.193, 360. / pow(2., 17)));
    _zoomScale.push_back(ZoomStruct(2000     ,      0.596, 360. / pow(2., 18)));
    _zoomScale.push_back(ZoomStruct(1000     ,      0.298, 360. / pow(2., 19)));
}

unsigned ZoomInfo::getCurrentZoomLevel()
{
    return _zoomLevel;
}

void ZoomInfo::zoomDecrement()
{
    if(_zoomLevel > 0)
    {
        _zoomLevel--;

        emit zoomChanged();
    }
}

void ZoomInfo::zoomIncrement()
{
    if(_zoomLevel < (MaxZoomLevel - 1))
    {
        _zoomLevel++;

        emit zoomChanged();
    }
}


void ZoomInfo::wheelEvent(QWheelEvent *wheelEvent)
{
    if(wheelEvent->delta() > 0)
    {
        zoomIncrement();
    }
    else
    {
        zoomDecrement();
    }

    //std::cout << "current zoom level is: " << _zoomLevel << ", current scale is: " << getCurrentScaleString().toStdString()
    //          << ", pos: " << wheelEvent->posF().x() << " " << wheelEvent->posF().y() << std::endl;
}

void ZoomInfo::keyPressEvent(QKeyEvent *keyEvent)
{
    switch (keyEvent->key())
    {
        case Qt::Key_Plus:
            zoomIncrement();
            break;

        case Qt::Key_Minus:
            zoomDecrement();
            break;

        default:
            break;
    }
}

unsigned ZoomInfo::getCurrentScale()
{
    if(_zoomLevel < _zoomScale.size())
    {
        return _zoomScale[_zoomLevel].scale;
    }

    return 0;
}

QString ZoomInfo::getCurrentScaleString()
{
    return "1:" + QString::number(getCurrentScale());
}

float ZoomInfo::getCurrentDegreeForTile()
{
    if(_zoomLevel < _zoomScale.size())
    {
        return _zoomScale[_zoomLevel].degree;
    }

    return 0;
}

void ZoomInfo::setCurrentZoomLevel(unsigned level)
{
    _zoomLevel = level;
}
