#ifndef OSMTILEDOWNLOADER_H
#define OSMTILEDOWNLOADER_H

#include <QProcess>
#include <QMutex>

class OSMTileDownloader : public QObject
{
        Q_OBJECT

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
        void setThreads(int threads);
        QString getBaseUrl() { return _baseWebRootUrl; }
        void setBaseUrl(QString url);
        QStringList getBaseUrlList() { return _baseWebRootUrllist; }

    public slots:
        void setDownloadingEnable(bool enabled);
        void startDownload();

    signals:
        void allItemIsDownloaded();
        void downloadItemIsDone();
        void downloadingEnable(bool value);
        void downloadedItem(int level, int col, int row);

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
        int _threads;
};

#endif // OSMTILEDOWNLOADER_H
