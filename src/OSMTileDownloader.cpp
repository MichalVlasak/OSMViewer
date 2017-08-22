#include "OSMTileDownloader.h"

#include <QDir>
#include <QThread>

#include <iostream>

const size_t OSMTileDownloader::MAX_QUEUE = 1000;

OSMTileDownloader::OSMTileDownloader(QObject *parent)
    : QObject(parent)
{
    _threads = QThread::idealThreadCount();

    _baseWebRootUrllist.append("140.211.167.105");
    _baseWebRootUrllist.append("http://tile.openstreetmap.org");

    _baseWebRootUrl = _baseWebRootUrllist.at(0);

    for(int i = 0; i < _threads; i++)
    {
        _processVector.push_back(newProcess());
    }
}

QProcess * OSMTileDownloader::newProcess()
{
    QProcess * proc = new QProcess(this);
    proc->close();

    QObject::connect(proc, SIGNAL(finished(int)), SLOT(processDone()));

    return proc;
}

void OSMTileDownloader::setBaseUrl(QString url)
{
    _baseWebRootUrl = url;
}

void OSMTileDownloader::setDownloadingEnable(bool enabled)
{
    _isDownloadingEnable = enabled;

    emit downloadingEnable(_isDownloadingEnable);
}

bool OSMTileDownloader::isDownloadingEnable()
{
    return _isDownloadingEnable;
}

QProcess * OSMTileDownloader::getFreeProcess()
{
    QMutexLocker lock(&_processMutex);

    for(size_t i = 0; i < _processVector.size(); i++)
    {
        QProcess * proc = _processVector[i];

        bool isOpen = proc->isOpen();

        if(isOpen == false)
        {
            return proc;
        }
    }

    return nullptr;
}

void OSMTileDownloader::addUrlToDownload(DownloadItem newItem, bool autoDownload)
{
    bool isPresent = false;

    QMutexLocker lock(&_mutex);

    for(const DownloadItem & item : _itemsToDownload)
    {
        if(item.level == newItem.level && item.column == newItem.column && item.row == newItem.row)
        {
            isPresent = true;
        }
    }

    if(isPresent == false)
    {
        _itemsToDownload.push_back(newItem);
        lock.unlock();
    }

    if(_itemsToDownload.size() > 0 && autoDownload == true)
    {
        startDownload();
    }
}

bool OSMTileDownloader::isFileExist(DownloadItem item)
{
    QFile file(item.fullPath);

    return file.exists();
}

void OSMTileDownloader::startDownload()
{
    QProcess * proc = getFreeProcess();

    if(proc != nullptr && _itemsToDownload.size() > 0)
    {
        DownloadItemsVector::iterator it = _itemsToDownload.begin();

        if(it != _itemsToDownload.end())
        {
            DownloadItem & item = *it;

            while(isFileExist(item) == true)
            {
                emit downloadedItem(item.level, item.column, item.row);

                //QMutexLocker lock(&_mutex);

                _itemsToDownload.erase(_itemsToDownload.begin());

                //lock.unlock();

                it = _itemsToDownload.begin();

                if(it != _itemsToDownload.end())
                {
                    item = *it;
                }
                else
                {
                    emit allItemIsDownloaded();
                    return;
                }
            }

            QDir levelDir(item.basePath + "/" + QString::number(item.level));
            if(levelDir.exists() == false)
            {
                levelDir.mkpath(levelDir.absolutePath());
            }

            QDir columnDir(levelDir.path() + "/" + QString::number(item.column));
            if(columnDir.exists() == false)
            {
                columnDir.mkpath(columnDir.absolutePath());
            }

            QString tileUrl = QString("%1/%2/%3/%4.png").arg(_baseWebRootUrl).arg(item.level).arg(item.column).arg(item.row);

            //std::cout << "start download" << std::endl;

            QString command = "wget " + tileUrl + " -O " + item.fullPath;
            proc->start(command);
            proc->startDetached(command);
            //proc->waitForFinished();

            //std::cout << "stop download" << std::endl;
        }
    }
}

void OSMTileDownloader::processDone()
{
    //std::cout << "process done" << std::endl;

    QProcess * proc = qobject_cast<QProcess*>(sender());

    if(proc != nullptr)
    {
        proc->close();

        // ak prislo znizenie poctu trackov tak procesy budem odoberat az tu, je to jednoduchsie
        if(_threads < _processVector.size())
        {
            QMutexLocker lock(&_processMutex);

            int idx = -1;

            for(size_t i = 0; i < _processVector.size(); i++)
            {
                if(_processVector[i] == proc)
                {
                    idx = i;
                    break;
                }
            }

            if(idx != -1)
            {
                _processVector.erase(_processVector.begin() + idx);
            }
        }

        emit downloadItemIsDone();

        startDownload();
    }
}

void OSMTileDownloader::cancelDownload()
{
    QMutexLocker lock(&_mutex);

    _itemsToDownload.clear();
}

bool OSMTileDownloader::isRunning()
{
    QMutexLocker lock(&_processMutex);

    for(size_t i = 0; i < _processVector.size(); i++)
    {
        QProcess * proc = _processVector[i];

        if(proc->isOpen() == true)
        {
            return true;
        }
    }

    return false;
}

void OSMTileDownloader::setThreads(size_t threads)
{
    QMutexLocker lock(&_processMutex);

    _threads = threads;

    // pridanie dalsich "vlakien" mozem urobit hned, odobratie budem riesit ked sa niektory z procesov ukonci
    if(threads > _processVector.size())
    {
        for(int i = _processVector.size(); i < _threads; i++)
        {
            _processVector.push_back(newProcess());
        }
    }

    lock.unlock();

    for(int i = 0; i < threads; i++)
    {
        startDownload();
    }
}

bool OSMTileDownloader::isFreeQueue()
{
    QMutexLocker lock(&_mutex);

    return _itemsToDownload.size() < MAX_QUEUE;
}
