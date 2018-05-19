#ifndef GPXMANAGER_H
#define GPXMANAGER_H

#include <QString>

#include "StoreConfigInterface.h"

class GpxManager : public StoreConfigInterface
{
    public:
        GpxManager();

    public:
        // interface zo StoreConfigInterface
        void storeConfig(QDomDocument & document, QDomElement & rootElement);
        bool restoreConfig(QDomDocument & document);

    public:
        const QString & getLastPathToGpxFiles() const;

        void loadGpxFiles(const QStringList & fileNames);

    private:
        QString _lastPathToGpxFiles;
};

#endif // GPXMANAGER_H
