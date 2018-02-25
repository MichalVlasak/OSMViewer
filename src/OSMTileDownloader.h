#ifndef OSMTILEDOWNLOADER_H
#define OSMTILEDOWNLOADER_H

#include <QProcess>
#include <QMutex>

#include "StoreConfigInterface.h"

class OSMTileDownloader : public QObject, public StoreConfigInterface
{
        Q_OBJECT

    public:
        static const size_t MAX_QUEUE;

    public:
        struct DownloadItem
        {
                int level;
                int column;
                int row;
                QString fullPath;
                QString basePath;
                bool isDownloading = false;
        };

    public:
        explicit OSMTileDownloader(QObject *parent = 0);

    public:
        bool isDownloadingEnable();
        void cancelDownload();
        void addUrlToDownload(DownloadItem newItem, bool autoDownload = true);
        bool isRunning();
        int getThreads() { return _threads; }
        void setThreads(size_t threads);
        QString getBaseUrl() { return _baseWebRootUrl; }
        void setBaseUrl(QString url);
        QStringList getBaseUrlList() { return _baseWebRootUrllist; }
        bool isFreeQueue();
        unsigned getSessionDownloadCount();
        unsigned getAllDownloadCount();

        // interface zo StoreConfigInterface
        void storeConfig(QDomDocument & document, QDomElement & rootElement);
        bool restoreConfig(QDomDocument & document);

    public slots:
        void setDownloadingEnable(bool enabled);
        void startDownload();

    signals:
        void allItemIsDownloaded();
        void downloadItemIsDone();
        void downloadingEnable(bool value);
        void downloadedItem(int level, int col, int row);
        void changeThreadsCount(int count);

    private:
        bool isFileExist(DownloadItem item);
        QProcess * getFreeProcess();
        QProcess * newProcess();

    private slots:
        void processDone();

    private:
        typedef std::vector<DownloadItem> DownloadItemsVector;
        typedef std::vector<QProcess*> ProcessVector;

    private:
        bool _isDownloadingEnable = false;
        DownloadItemsVector _itemsToDownload;
        ProcessVector _processVector;
        QMutex _mutex;
        QMutex _processMutex;
        QString _baseWebRootUrl;
        QStringList _baseWebRootUrllist;
        size_t _threads;
        unsigned _sessionDownloadCount = 0;
        unsigned _allDownloadCount = 0;
        bool _isDownloadCanceled = false;
};

#endif // OSMTILEDOWNLOADER_H
