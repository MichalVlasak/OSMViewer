#include "CenterPointsManager.h"

CenterPointsManager::CenterPointsManager()
{
    //_homePosition = CenterPointStruct(QPointF(0.0, 0.0), "Zero Point", -1);
    _homePosition = CenterPointStruct(QPointF(17.1097222222, 48.1438888889), "Bratislava", 12);
}

const CenterPointStruct & CenterPointsManager::getHomeCenterPoint() const
{
    return _homePosition;
}

const CenterPointsManager::CenterPointsVector & CenterPointsManager::getCenterPointsVector() const
{
    return _pointsVector;
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
    }
}
