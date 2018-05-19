#ifndef GPXMANAGER_H
#define GPXMANAGER_H

#include <QString>

#include "StoreConfigInterface.h"

class GpxManager : public StoreConfigInterface
{
    public:
        GpxManager();
        virtual ~GpxManager() = default;

        static const int ErrorId = -1;

        struct GpxItem
        {
                QString filePath;
                QString description;
                int fileId = ErrorId;
        };

        typedef std::vector<GpxItem> GpxVector;

    public:
        // interface zo StoreConfigInterface
        void storeConfig(QDomDocument & document, QDomElement & rootElement);
        bool restoreConfig(QDomDocument & document);

    public:
        const QString & getLastPathToGpxFiles() const;
        const GpxVector & getGpxVector() const;

        void loadGpxFile(const QString & filePath);
        void loadGpxFiles(const QStringList & filePaths);

        void removeGpxFile(int fileId);
        void removeAll();

    private:
        int getNextItemId();

    private:
        QString _lastPathToGpxFiles;
        GpxVector _gpxVector;
        static int itemIdCounter;
};

#endif // GPXMANAGER_H
