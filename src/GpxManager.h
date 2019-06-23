#ifndef GPXMANAGER_H
#define GPXMANAGER_H

#include <QString>

#include "StoreConfigInterface.h"

class GpxManager : public QObject, public StoreConfigInterface
{
        Q_OBJECT

    public:
        GpxManager(QObject * parent = nullptr);
        virtual ~GpxManager() = default;

        static const int ErrorId = -1;

        struct Point
        {
                double lat = 0.;
                double lon = 0.;
                QVariant elevation; // vklada sa double
                QVariant time;
                QVariant temperature; // vklada sa float
                QVariant cadention; // vklada sa int
                QVariant heartRate; // vklada sa int
        };

        typedef std::vector<Point> PointVector;

        struct GpxItem
        {
                QString filePath;
                QString name;
                QString authorName;
                QString description;
                QVariant startTime;
                time_t tripTime = 0;
                int fileId = ErrorId;
                PointVector pointVector;
                double totalDistance = 0.; // v metroch

                int biggestElevetionIdx = ErrorId;
                bool showBiggestElevetion = false;

                int biggestTemperatureIdx = ErrorId;
                bool showBiggestTemperature = false;

                int biggestCadentionIdx = ErrorId;
                bool showBiggestCadention = false;

                int biggestHeartRateIdx = ErrorId;
                bool showBiggestHeartRate = false;

                bool highlight = false;
        };

        typedef std::vector<GpxItem> GpxVector;
        typedef std::vector<int> GpxIdVector;

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

        bool isAllLoaaded() const;

        static void downloadTilesForGpx(const GpxItem & gpxItem);

    public slots:
        void cancelLoadGpx();

    signals:
        void gpxCurrentLoadingSignals(QString filePath);
        void gpxWasLoadedSignals(int fileId);
        void gpxStatusLoad(int allCount, int currentCounter);
        void gpxStatusAllLoaded();

    private:
        int getNextItemId();
        void loadXml(const QString & filePath, GpxItem & gpxItem);
        void loadGpxFilesInFuture();

    private:
        typedef QFutureWatcher<void> GpxLoaderWatcher;

    private slots:
        void gpxWasLoaded();

    private:
        QString _lastPathToGpxFiles;
        GpxVector _gpxVector;
        static int itemIdCounter;
        QStringList _filePathsToLoadInFuture;
        bool _isAllLoaded = false;
        volatile bool _cancelLoad = false;
};

typedef std::shared_ptr<GpxManager> GpxManagerPtr;

#endif // GPXMANAGER_H
