#include "CenterPointsManager.h"

CenterPointsManager::CenterPointsManager(QObject * parent)
    : QObject(parent)
{
    _homePosition = CenterPointStruct(QPointF(0.0, 0.0), "Zero Point", -1);

//    addCenterPoint(CenterPointStruct(QPointF(17.1097222222, 48.1438888889), "Bratislava", 12));
//    addCenterPoint(CenterPointStruct(QPointF(14.4211111111, 50.0872222222), "Praha"     , 12));
//    addCenterPoint(CenterPointStruct(QPointF(16.3730555556, 48.2083333333), "Vieden"    , 12));
//    addCenterPoint(CenterPointStruct(QPointF(19.0502777778, 47.4719444444), "Budapest"  , 12));
//    addCenterPoint(CenterPointStruct(QPointF(21.0083333333, 52.2322222222), "Varsava"   , 12));
//    addCenterPoint(CenterPointStruct(QPointF(13.4         , 52.5166666667), "Berlin"    , 11));
}

const CenterPointStruct & CenterPointsManager::getHomeCenterPoint() const
{
    return _homePosition;
}

void CenterPointsManager::setHomeCenterPoint(const CenterPointStruct & point)
{
    _homePosition = point;

    emit homePointWasChanged();
}

const CenterPointsManager::CenterPointsVector & CenterPointsManager::getCenterPointsVector() const
{
    return _pointsVector;
}

bool compare(const CenterPointStruct & p1, const CenterPointStruct & p2)
{
    return (p1.name.compare(p2.name) < 0) ? true : false;
}

void CenterPointsManager::addCenterPoint(const CenterPointStruct &centerPoint)
{
    bool isPointPresent = false;

    for(const CenterPointStruct & point : _pointsVector)
    {
        if(point.name.compare(centerPoint.name) == 0)
        {
            isPointPresent = true;
            break;
        }
    }

    if(isPointPresent == false)
    {
        _pointsVector.push_back(centerPoint);

        std::sort(_pointsVector.begin(), _pointsVector.end(), compare);

        emit pointsWasAdded();
    }
}

void CenterPointsManager::removeCenterPoint(const CenterPointStruct &centerPoint)
{
    bool isPointPresent = false;
    size_t i = 0;

    for(i = 0; i < _pointsVector.size(); i++)
    {
        if(_pointsVector[i].name.compare(centerPoint.name) == 0)
        {
            isPointPresent = true;
            break;
        }
    }

    if(isPointPresent == true)
    {
        _pointsVector.erase(_pointsVector.begin() + i);

        std::sort(_pointsVector.begin(), _pointsVector.end(), compare);

        emit pointsWasRemoved();
    }
}
