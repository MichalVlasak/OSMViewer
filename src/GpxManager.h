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

        struct Point
        {
                double lat = 0.;
                double lon = 0.;
                double elevation = 0.;
                QVariant time;
                int temperature = 0;
                int cadention = 0;
                int heartRate = 0;
        };

        typedef std::vector<Point> PointVector;

        struct GpxItem
        {
                QString filePath;
                QString name;
                QString authorName;
                QString description;
                QVariant time;
                int fileId = ErrorId;
                PointVector pointVector;
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
        void loadXml(const QString & filePath, GpxItem & gpxItem);

    private:
        QString _lastPathToGpxFiles;
        GpxVector _gpxVector;
        static int itemIdCounter;
};

#endif // GPXMANAGER_H
