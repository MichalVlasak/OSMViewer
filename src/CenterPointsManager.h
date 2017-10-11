#ifndef CENTERPOINTSMANAGER_H
#define CENTERPOINTSMANAGER_H

#include "CenterPointStruct.h"

#include <vector>
#include <QObject>

class CenterPointsManager : public QObject
{
        Q_OBJECT

    public:
        typedef std::vector<CenterPointStruct> CenterPointsVector;

    public:
        CenterPointsManager(QObject * parent = nullptr);

    public:
        const CenterPointStruct & getHomeCenterPoint() const;
        void setHomeCenterPoint(const CenterPointStruct & point);
        const CenterPointsVector & getCenterPointsVector() const;
        void addCenterPoint(const CenterPointStruct & centerPoint);
        void removeCenterPoint(const CenterPointStruct & centerPoint);

    signals:
        void homePointWasChanged();
        void pointsWasAdded();
        void pointsWasRemoved();

    private:
        CenterPointStruct _homePosition;
        CenterPointsVector _pointsVector;
};

#endif // CENTERPOINTSMANAGER_H
