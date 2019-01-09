#ifndef CENTERPOINTSMANAGER_H
#define CENTERPOINTSMANAGER_H

#include "CenterPointStruct.h"
#include "StoreConfigInterface.h"

#include <vector>

#include <QObject>
#include <QtXml>


class CenterPointsManager : public QObject, public StoreConfigInterface
{
        Q_OBJECT

    public:
        typedef std::vector<CenterPointStruct> CenterPointsVector;
        typedef std::map<QString, CenterPointsVector> CenterPointsMap; // meno skupiny, zoznam bodov pre skupinu

    public:
        explicit CenterPointsManager(QObject * parent = nullptr);

    public:
        const CenterPointStruct & getHomeCenterPoint() const;
        void setHomeCenterPoint(const CenterPointStruct & point);
        const CenterPointsVector * getCenterPointsVector(const QString & groupName) const;
        const CenterPointsMap & getCenterPointsMap() const;
        void addCenterPoint(const QString & groupName, const CenterPointStruct & centerPoint, bool emitSignal);
        void removeCenterPoint(const QString & groupName, const CenterPointStruct & centerPoint, bool emitSignal);
        void removeAllCenterPoints();

        void importPoints();
        void exportAllGroups();
        void exportGroup(const QString & groupName);

        const QString & getImportExportLastPath() const;
        void setImportExportLastPath(const QString & path);

        static void storePoint(const CenterPointStruct & point, QDomElement & element, QDomDocument & doc);

        bool createNewGroup(const QString & groupName);
        bool changeGroupName(const QString & oldGroupName, const QString & newGroupName);
        bool removeGroup(const QString & groupName);

        // interface zo StoreConfigInterface
        void storeConfig(QDomDocument & document, QDomElement & rootElement);
        bool restoreConfig(QDomDocument & document);

    signals:
        void homePointWasChanged();
        void pointsWasAdded();
        void pointsWasRemoved();

    private:
        void prepareXmlDocument(const QString & fileName, QDomDocument & doc, QDomElement & groupsElement);
        void savePointsToXml(CenterPointsMap::iterator it, QDomDocument & doc, QDomElement & groupsElement);
        void importXml(const QString & fileName);
        void importCsv(const QString & fileName);

    private:
        CenterPointStruct _homePosition;
        CenterPointsMap _pointsMap;
        QString _importExportLastPath;
};

#endif // CENTERPOINTSMANAGER_H
