#ifndef CENTERPOINTSMANAGER_H
#define CENTERPOINTSMANAGER_H

#include "CenterPointStruct.h"

#include <vector>

class CenterPointsManager
{
    public:
        typedef std::vector<CenterPointStruct> CenterPointsVector;

    public:
        CenterPointsManager();

    public:
        const CenterPointStruct & getHomeCenterPoint() const;
        const CenterPointsVector & getCenterPointsVector() const;
        void addCenterPoint(const CenterPointStruct & centerPoint);

    private:
        CenterPointStruct _homePosition;
        CenterPointsVector _pointsVector;
};

#endif // CENTERPOINTSMANAGER_H
