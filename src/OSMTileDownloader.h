#ifndef OSMTILEDOWNLOADER_H
#define OSMTILEDOWNLOADER_H

#include <QMutex>
#include <QtNetwork>

#include "StoreConfigInterface.h"

class OSMTileDownloader : public QObject, public StoreConfigInterface
{
        Q_OBJECT

    public:
        static const int MAX_QUEUE;

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
        OSMTileDownloader(const QString & appName, QObject *parent = nullptr);
        virtual ~OSMTileDownloader() = default;

    public:
        bool isDownloadingEnable();
        void cancelDownload();
        bool addDownloadItem(const DownloadItem & itemStruct);
        bool isFreeQueue();
        bool isRunning();
        int getThreads() { return _threads; }
        void setThreads(int threads);
        unsigned getAllDownloadCount();
        QString getBaseUrl() { return _baseWebRootUrl; }
        void setBaseUrl(QString url);
        QStringList getBaseUrlList() { return _baseWebRootUrllist; }

        // interface zo StoreConfigInterface
        void storeConfig(QDomDocument & document, QDomElement & rootElement);
        bool restoreConfig(QDomDocument & document);

    public slots:
        void setDownloadingEnable(bool enabled);

    signals:
        void downloadingEnable(bool value);
        void changeThreadsCount(int count);
        void downloadedItem(int level, int col, int row);
        void downloadItemIsDone();
        void allItemIsDownloaded();
        void doDownladSignal(QUrl url);
        void changeBaseUrl(QString url);

    private:
        bool saveToDisk(const QUrl & url, QIODevice *data, QString & fileName);
        bool isHttpRedirect(QNetworkReply *reply);

    private slots:
        void downloadFinished(QNetworkReply *reply);
        void sslErrors(const QList<QSslError> &errors);
        void doDownload(QUrl url);

    private:
        QVector<QNetworkAccessManager*> _managers;
        QVector<QNetworkReply *> _currentDownloads;
        QVector<DownloadItem> _downloadingItems;
        QString _baseWebRootUrl;
        QStringList _baseWebRootUrllist;
        QMutex _mutex;
        QMutex _threadMutex;
        bool _isDownloadingEnable = true;
        int _threads;
        unsigned _allDownloadCount = 0;
        QString _appName;
        int _lastManager = 0;
};

#endif // OSMTILEDOWNLOADER_H
