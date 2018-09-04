#include "OSMTileDownloader.h"
#include "AppSettings.h"
#include "MainWindow.h"

#include <QDir>
#include <QThread>
#include <QTimer>

#include <iostream>

const int OSMTileDownloader::MAX_QUEUE = 100;

OSMTileDownloader::OSMTileDownloader(const QString & appName, QObject *parent)
    : QObject(parent),
      _appName(appName)
{
    _threads = QThread::idealThreadCount();
    _allDownloadCount = 0;

    _baseWebRootUrllist.append("http://140.211.167.105");
    _baseWebRootUrllist.append("http://tile.openstreetmap.org");

    _baseWebRootUrl = _baseWebRootUrllist.at(0);

    for(int i = 0; i < _threads; i++)
    {
        _managers.push_back(new QNetworkAccessManager(this));
        QObject::connect(_managers[i], SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
    }

    QObject::connect(this, SIGNAL(doDownladSignal(QUrl)), SLOT(doDownload(QUrl)));
}

void OSMTileDownloader::setBaseUrl(QString url)
{
    _baseWebRootUrl = url;

    emit changeBaseUrl(_baseWebRootUrl);
}

bool OSMTileDownloader::isFreeQueue()
{
    QMutexLocker lock(&_mutex);

    if(_downloadingItems.size() < MAX_QUEUE)
    {
        return true;
    }

    return false;
}

void OSMTileDownloader::doDownload(QUrl url)
{
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray::fromStdString("User-Agent"), _appName.toUtf8());

    QMutexLocker lockThread(&_threadMutex);

    if(_lastManager > _managers.size())
    {
        _lastManager = 0;
    }

    QNetworkReply *reply = _managers[_lastManager++]->get(request);

    if(_lastManager >= _threads)
    {
        _lastManager = 0;
    }

#if QT_CONFIG(ssl)
    QObject::connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
#endif

    QMutexLocker lock(&_mutex);
    _currentDownloads.append(reply);
}

bool OSMTileDownloader::addDownloadItem(const OSMTileDownloader::DownloadItem & itemStruct)
{
    bool isPresent = false;

    for(const DownloadItem & item : _downloadingItems)
    {
        if(item.level == itemStruct.level &&
           item.column == itemStruct.column &&
           item.row == itemStruct.row)
        {
            isPresent = true;

            break;
        }
    }

    if(isPresent == false)
    {
        QString tileUrl = QString("%1/%2/%3/%4.png").arg(_baseWebRootUrl).arg(itemStruct.level).arg(itemStruct.column).arg(itemStruct.row);

        QUrl url = QUrl(tileUrl);

        {
            QMutexLocker lock(&_mutex);
            _downloadingItems.append(itemStruct);
        }

        emit doDownladSignal(url);
    }

    return true;
}

bool OSMTileDownloader::saveToDisk(const QUrl &url, QIODevice *data, QString & fileName)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
    {
        basename = "???unknown???";
    }
    else
    {
        QString origUrl = url.url();

        QMutexLocker lock(&_mutex);

        bool isFinded = false;

        for(int i = 0; i < _downloadingItems.size(); i++)
        {
            const DownloadItem & itemStruct = _downloadingItems[i];

            QString tileUrl = QString("%1/%2/%3/%4.png").arg(_baseWebRootUrl).arg(itemStruct.level).arg(itemStruct.column).arg(itemStruct.row);

            if(origUrl.compare(tileUrl) == 0)
            {
                QString directory = QString("%1/%2/%3").arg(itemStruct.basePath).arg(itemStruct.level).arg(itemStruct.column);

                if(QDir(directory).exists() == false)
                {
                    QDir().mkpath(directory);
                }

                emit downloadedItem(itemStruct.level, itemStruct.column, itemStruct.row);

                basename = itemStruct.fullPath;
                _downloadingItems.remove(i);

                isFinded = true;

                break;
            }
        }

        if(isFinded == false)
        {
            if(_downloadingItems.size() > 0)
            {
                basename = _downloadingItems[0].basePath + path;
            }
        }

        QFile file(basename);
        if (!file.open(QIODevice::WriteOnly))
        {
            std::cerr << "Could not open " << qPrintable(basename) << " for writing: " << qPrintable(file.errorString()) << std::endl;
            return false;
        }

        file.write(data->readAll());
        file.close();

        fileName = basename;

        return true;
    }

    return false;
}

bool OSMTileDownloader::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
            || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void OSMTileDownloader::sslErrors(const QList<QSslError> &sslErrors)
{
#if QT_CONFIG(ssl)
    for (const QSslError &error : sslErrors)
    {
        std::cerr << "SSL error: " << qPrintable(error.errorString()) << std::endl;
    }
#else
    Q_UNUSED(sslErrors);
#endif
}

void OSMTileDownloader::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();

    if (reply->error())
    {
        std::cerr << "Download of " << url.toEncoded().constData() << " failed: " << qPrintable(reply->errorString()) << std::endl;
    }
    else
    {
        if (isHttpRedirect(reply))
        {
            std::cerr << "Request was redirected." << std::endl;
        }
        else
        {
            QString fileName;

            if (saveToDisk(url, reply, fileName))
            {
                std::cout << "Download of " << url.toEncoded().constData() << " succeeded (saved to " << qPrintable(fileName) << ")" << std::endl;
            }
        }
    }

    {
        QMutexLocker lock(&_mutex);

        _currentDownloads.removeAll(reply);
    }

    reply->deleteLater();

    _allDownloadCount++;

    if (_currentDownloads.isEmpty())
    {
        // all downloads finished
        emit allItemIsDownloaded();
    }

    emit downloadItemIsDone();
}

void OSMTileDownloader::setDownloadingEnable(bool enabled)
{
    _isDownloadingEnable = enabled;

    emit downloadingEnable(_isDownloadingEnable);

    if(enabled == false)
    {
        cancelDownload();
    }
}

bool OSMTileDownloader::isDownloadingEnable()
{
    return _isDownloadingEnable;
}

bool OSMTileDownloader::isRunning()
{
    QMutexLocker lock(&_mutex);

    for(QNetworkReply * reply : _currentDownloads)
    {
        if(reply != nullptr && reply->isRunning() == true)
        {
            return true;
        }
    }

    return false;
}

void OSMTileDownloader::cancelDownload()
{
    QMutexLocker lock(&_mutex);

    for(QNetworkReply * reply : _currentDownloads)
    {
        if(reply != nullptr)
        {
            reply->deleteLater();
        }
    }

    _currentDownloads.clear();
    _downloadingItems.clear();
}

void OSMTileDownloader::setThreads(int threads)
{
    if(threads == _threads)
    {
        return;
    }
    else if(threads > _threads)
    {
        QMutexLocker lockThread(&_threadMutex);

        for(int i = _threads; i < threads; i++)
        {
            _managers.push_back(new QNetworkAccessManager(this));
            QObject::connect(_managers[i], SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
        }
    }
    else if(threads < _threads)
    {
        if(isRunning() == false)
        {
            QMutexLocker lockThread(&_threadMutex);

            for(int i = threads; i < _threads; i++)
            {
                QNetworkAccessManager * manager = _managers[i];

                if(manager != nullptr)
                {
                    delete manager;
                    manager = nullptr;
                }
            }

            _managers.resize(threads);
        }
    }

    _threads = threads;

    emit changeThreadsCount(_threads);
}

unsigned OSMTileDownloader::getAllDownloadCount()
{
    return _allDownloadCount;
}

void OSMTileDownloader::storeConfig(QDomDocument &document, QDomElement &rootElement)
{
    QDomElement downloaderElement = document.createElement("OSMDownloaderSettings");
    rootElement.appendChild(downloaderElement);

    QDomElement downloadEnableElement = document.createElement("EnabledDownloading");
    downloaderElement.appendChild(downloadEnableElement);
    QDomText downloadEnableText = document.createTextNode(QString::number(isDownloadingEnable()));
    downloadEnableElement.appendChild(downloadEnableText);

    QDomElement threadsCountElement = document.createElement("ThreadsCount");
    downloaderElement.appendChild(threadsCountElement);
    QDomText threadsCountText = document.createTextNode(QString::number(getThreads()));
    threadsCountElement.appendChild(threadsCountText);

    QDomElement baseUrlElement = document.createElement("BaseUrl");
    downloaderElement.appendChild(baseUrlElement);
    QDomText baseUrlText = document.createTextNode(getBaseUrl());
    baseUrlElement.appendChild(baseUrlText);
}

bool OSMTileDownloader::restoreConfig(QDomDocument &document)
{
    bool result = false;

    QDomElement rootElem = document.firstChildElement("OSMViewer");

    if(rootElem.isNull() == false)
    {
        QDomNodeList downloadSettingsNodes = rootElem.elementsByTagName("OSMDownloaderSettings");

        for(int iMap = 0; iMap < downloadSettingsNodes.size(); iMap++)
        {
            QDomNode downloadSettingNode = downloadSettingsNodes.at(iMap);

            if(downloadSettingNode.isNull() == false)
            {
                QString value = AppSettings::getValueString(downloadSettingNode, "EnabledDownloading");

                if(value.isEmpty() == false)
                {
                    setDownloadingEnable(value.toInt());
                    result = true;
                }

                value = AppSettings::getValueString(downloadSettingNode, "ThreadsCount");

                if(value.isEmpty() == false)
                {
                    setThreads(value.toInt());
                    result = true;
                }

                value = AppSettings::getValueString(downloadSettingNode, "BaseUrl");

                if(value.isEmpty() == false)
                {
                    setBaseUrl(value);
                    result = true;
                }
            }
        }
    }

    return result;
}
