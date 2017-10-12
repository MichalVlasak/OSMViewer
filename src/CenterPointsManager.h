#ifndef CENTERPOINTSMANAGER_H
#define CENTERPOINTSMANAGER_H

#include "CenterPointStruct.h"

#include <vector>

#include <QObject>
#include <QtXml>


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
        void removeAllCenterPoints();

        void importPoints();
        void exportPoints();

        const QString & getImportExportLastPath() const;
        void setImportExportLastPath(const QString & path);

        static void storePoint(const CenterPointStruct & point, QDomElement & element, QDomDocument & doc);

    signals:
        void homePointWasChanged();
        void pointsWasAdded();
        void pointsWasRemoved();

    private:
        CenterPointStruct _homePosition;
        CenterPointsVector _pointsVector;
        QString _importExportLastPath;
};

#endif // CENTERPOINTSMANAGER_H
