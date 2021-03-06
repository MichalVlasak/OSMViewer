#include "GpxManager.h"
#include "AppSettings.h"
#include "MainWindow.h"
#include "GpxInfoFileWidgetConfig.h"

#include <iostream>
#include <QFileInfo>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>

int GpxManager::itemIdCounter = 0;

GpxManager::GpxManager(QObject * parent)
    : QObject(parent)
{
    _lastPathToGpxFiles = ".";
}

void GpxManager::storeConfig(QDomDocument &document, QDomElement &rootElement)
{
    QDomElement gpxElement = document.createElement("GPXManager");
    rootElement.appendChild(gpxElement);

    QDomElement gpxLastPathElement = document.createElement("LastPathToGpxFiles");
    gpxElement.appendChild(gpxLastPathElement);
    QDomText gpxLastPathText = document.createTextNode(_lastPathToGpxFiles);
    gpxLastPathElement.appendChild(gpxLastPathText);

    QDomElement gpxFilesElements = document.createElement("GpxFiles");
    gpxElement.appendChild(gpxFilesElements);

    for(const GpxItem & item : _gpxVector)
    {
        QDomElement filePathElement = document.createElement("FilePath");
        gpxFilesElements.appendChild(filePathElement);
        QDomText filePathText = document.createTextNode(item.filePath);
        filePathElement.appendChild(filePathText);
    }

    GpxInfoFileWidgetConfig infoFileCfg;

    infoFileCfg.storeConfig(document, rootElement);
}

bool GpxManager::restoreConfig(QDomDocument &document)
{
    bool result = false;

    GpxInfoFileWidgetConfig infoFileCfg;

    infoFileCfg.restoreConfig(document);

    QDomElement rootElem = document.firstChildElement("OSMViewer");

    if(rootElem.isNull() == false)
    {
        QDomNodeList gpxNodes = rootElem.elementsByTagName("GPXManager");

        for(int i = 0; i < gpxNodes.size(); i++)
        {
            QDomNode gpxNode = gpxNodes.at(i);

            if(gpxNode.isNull() == false)
            {
                QString value = AppSettings::getValueString(gpxNode, "LastPathToGpxFiles");

                if(value.isEmpty() == false)
                {
                    _lastPathToGpxFiles = value;
                    result = true;
                }
            }

            QDomNodeList gpxFilesNodeList = gpxNode.toElement().elementsByTagName("GpxFiles");

            QStringList gpxFiles;

            for(int iGpxFiles = 0; iGpxFiles < gpxFilesNodeList.size(); iGpxFiles++)
            {
                QDomNode gpxFileNode = gpxFilesNodeList.at(iGpxFiles);

                if(gpxFileNode.isNull() == false)
                {
                    QDomNodeList filesPathNodeList = gpxFileNode.toElement().elementsByTagName("FilePath");

                    for(int iFilePath = 0; iFilePath < filesPathNodeList.size(); iFilePath++)
                    {
                        QDomNode filePathNode = filesPathNodeList.at(iFilePath);

                        if(filePathNode.isNull() == false)
                        {
                            QString filePath = filePathNode.toElement().text();

                            if(filePath.isEmpty() == false)
                            {
                                gpxFiles.push_back(filePath);
                                result = true;
                            }
                        }
                    }
                }
            }

            if(_cancelLoad == false)
            {
                loadGpxFiles(gpxFiles);
            }
            else
            {
                break;
            }
        }
    }

    return result;
}

int GpxManager::getNextItemId()
{
    return itemIdCounter++;
}

const QString & GpxManager::getLastPathToGpxFiles() const
{
    return _lastPathToGpxFiles;
}

void GpxManager::gpxWasLoaded()
{
    emit gpxStatusAllLoaded();

    _isAllLoaded = true;
}

void GpxManager::loadGpxFilesInFuture()
{
    int counter = 0;

    for(const QString & filePath : _filePathsToLoadInFuture)
    {
        if(_cancelLoad == false)
        {
            loadGpxFile(filePath);

            emit gpxStatusLoad(_filePathsToLoadInFuture.size(), ++counter);
        }
        else
        {
            break;
        }
    }
}

void GpxManager::loadGpxFiles(const QStringList &filePaths)
{
    _filePathsToLoadInFuture = filePaths;
    _cancelLoad = false;

    GpxLoaderWatcher * watcher = new GpxLoaderWatcher(this);

    QObject::connect(watcher, SIGNAL(finished()), SLOT(gpxWasLoaded()));
    watcher->setFuture(QtConcurrent::run(this, &GpxManager::loadGpxFilesInFuture));
}

void GpxManager::loadGpxFile(const QString &filePath)
{
    if(filePath.isEmpty() == false)
    {
        QFileInfo fileInfo(filePath);

        if(fileInfo.exists() == true)
        {
            _lastPathToGpxFiles = fileInfo.dir().path();

            GpxItem item;

            item.description = "not loaded";
            item.filePath = filePath;
            item.fileId = getNextItemId();

            emit gpxCurrentLoadingSignals(item.filePath);

            loadXml(filePath, item);

            _gpxVector.push_back(item);

            emit gpxWasLoadedSignals(item.fileId);
        }
    }
}

const GpxManager::GpxVector & GpxManager::getGpxVector() const
{
    return _gpxVector;
}

void GpxManager::removeGpxFile(int fileId)
{
    for(size_t i = 0; i < _gpxVector.size(); i++)
    {
        if(_gpxVector[i].fileId == fileId)
        {
            _gpxVector.erase(_gpxVector.begin() + i);

            break;
        }
    }
}

void GpxManager::removeAll()
{
    _gpxVector.clear();
}

QString getValueFromNode(QDomNode & node, const QString & nodeName)
{
    QDomNodeList nameNodeList = node.toElement().elementsByTagName(nodeName);
    QString value;

    if(nameNodeList.size() > 0)
    {
        QDomNode nameNode = nameNodeList.at(0);

        if(nameNode.isNull() == false)
        {
            value = nameNode.toElement().text();
        }
    }

    return value;
}

void GpxManager::loadXml(const QString &filePath, GpxItem & gpxItem)
{
    QDomDocument document;
    QFile file(filePath);

    if (document.setContent(&file) == false)
    {
        QMessageBox::warning(MainWindow::getInstance(), QObject::tr("Cannot load!"), QObject::tr("Cannot load file ") + filePath + QObject::tr("!"));

        return;
    }

    QDomElement rootElem = document.firstChildElement("gpx");

    if(rootElem.isNull() == false)
    {
        QString attributes = rootElem.attribute("xmlns:gpxtpx");
        QString nsPrefix = "gpxtpx";

        if(attributes.isEmpty() == true)
        {
            attributes = rootElem.attribute("xmlns:ns3");
            nsPrefix = "ns3";
        }

        QDomNodeList metadataNodes = rootElem.elementsByTagName("metadata");

        if(metadataNodes.size() > 0)
        {
            QDomNode metadataNode = metadataNodes.at(0);

            if(metadataNode.isNull() == false)
            {
                gpxItem.name = getValueFromNode(metadataNode, "name");
                gpxItem.description = getValueFromNode(metadataNode, "desc");
                QString timeString = getValueFromNode(metadataNode, "time");

                if(timeString.isEmpty() == false)
                {
                    QDateTime time = QDateTime::fromString(timeString, Qt::ISODate);
                    gpxItem.startTime = time;
                }

                QDomNodeList authorNodeList = metadataNode.toElement().elementsByTagName("author");

                if(authorNodeList.size() > 0)
                {
                    QDomNode authorNode = authorNodeList.at(0);

                    if(authorNode.isNull() == false)
                    {
                        gpxItem.authorName = getValueFromNode(authorNode, "name");
                    }
                }
            }
        }

        QDomNodeList trkNodes = rootElem.elementsByTagName("trk");

        if(trkNodes.size() > 0)
        {
            QDomNode trkNode = trkNodes.at(0);

            if(trkNode.isNull() == false)
            {
                if(gpxItem.name.isEmpty() == true)
                {
                    gpxItem.name = getValueFromNode(trkNode, "name");
                }

                QDomNodeList trksegNodes = trkNode.toElement().elementsByTagName("trkseg");

                if(trksegNodes.size() > 0)
                {
                    QDomNode trksegNode = trksegNodes.at(0);

                    if(trksegNode.isNull() == false)
                    {
                        QDomNodeList trkptNodes = trksegNode.toElement().elementsByTagName("trkpt");

                        int size = trkptNodes.size();
                        double maxElevation = 0.;
                        float maxTemperature = 0.;
                        int maxCadention = 0;
                        int maxHeartRate = 0;

                        for(int i = 0; i < size; i++)
                        {
                            QDomNode trkptNode = trkptNodes.at(i);

                            if(trkptNode.isNull() == false)
                            {
                                if(i == 0 && gpxItem.startTime.isNull() == true)
                                {
                                    QString timeString = getValueFromNode(trkptNode, "time");

                                    if(timeString.isEmpty() == false)
                                    {
                                        QDateTime time = QDateTime::fromString(timeString, Qt::ISODate);
                                        gpxItem.startTime = time;
                                    }
                                }

                                QString elevationString = getValueFromNode(trkptNode, "ele");
                                QString timeString = getValueFromNode(trkptNode, "time");
                                Point point;

                                if(elevationString.isEmpty() == false)
                                {
                                    bool isOk = false;
                                    double elevation = elevationString.toDouble(&isOk);

                                    if(isOk == true)
                                    {
                                        point.elevation = elevation;

                                        if(maxElevation < elevation)
                                        {
                                            maxElevation = elevation;

                                            gpxItem.biggestElevetionIdx = i;
                                        }
                                    }
                                }

                                if(timeString.isEmpty() == false)
                                {
                                    QDateTime time = QDateTime::fromString(timeString, Qt::ISODate);
                                    point.time = time;
                                }

                                QString latString = trkptNode.attributes().namedItem("lat").nodeValue();
                                QString lonString = trkptNode.attributes().namedItem("lon").nodeValue();

                                if(latString.isEmpty() == false && lonString.isEmpty() == false)
                                {
                                    bool isLatOk = false;
                                    bool isLonOk = false;

                                    double lat = latString.toDouble(&isLatOk);
                                    double lon = lonString.toDouble(&isLonOk);

                                    if(isLatOk == true && isLonOk == true)
                                    {
                                        point.lat = lat;
                                        point.lon = lon;
                                    }
                                }

                                QDomNodeList extensionsNodes = trkptNode.toElement().elementsByTagName("extensions");

                                if(extensionsNodes.size() > 0)
                                {
                                    QDomNode extensionsNode = extensionsNodes.at(0);

                                    if(extensionsNode.isNull() == false)
                                    {
                                        QDomNodeList trkptExtensionNodes = extensionsNode.toElement().elementsByTagName(nsPrefix + ":TrackPointExtension");

                                        if(trkptExtensionNodes.size() > 0)
                                        {
                                            QDomNode trkptExtensionNode = trkptExtensionNodes.at(0);

                                            if(trkptExtensionNode.isNull() == false)
                                            {
                                                QString temperatureString = getValueFromNode(trkptExtensionNode, nsPrefix + ":atemp");
                                                QString heartRateString = getValueFromNode(trkptExtensionNode, nsPrefix + ":hr");
                                                QString cadentionString = getValueFromNode(trkptExtensionNode, nsPrefix + ":cad");

                                                if(temperatureString.isEmpty() == false)
                                                {
                                                    bool isOk = false;
                                                    float temperature = temperatureString.toFloat(&isOk);

                                                    if(isOk == true)
                                                    {
                                                        point.temperature = temperature;

                                                        if(maxTemperature < temperature)
                                                        {
                                                            maxTemperature = temperature;

                                                            gpxItem.biggestTemperatureIdx = i;
                                                        }
                                                    }
                                                }

                                                if(heartRateString.isEmpty() == false)
                                                {
                                                    bool isOk = false;
                                                    int heartRate = heartRateString.toInt(&isOk);

                                                    if(isOk == true)
                                                    {
                                                        point.heartRate = heartRate;

                                                        if(maxHeartRate < heartRate)
                                                        {
                                                            maxHeartRate = heartRate;

                                                            gpxItem.biggestHeartRateIdx = i;
                                                        }
                                                    }
                                                }

                                                if(cadentionString.isEmpty() == false)
                                                {
                                                    bool isOk = false;
                                                    int cadention = cadentionString.toInt(&isOk);

                                                    if(isOk == true)
                                                    {
                                                        point.cadention = cadention;

                                                        if(maxCadention < cadention)
                                                        {
                                                            maxCadention = cadention;

                                                            gpxItem.biggestCadentionIdx = i;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                gpxItem.pointVector.push_back(point);
                            }
                        }
                    }
                }
            }
        }
    }
}

void GpxManager::downloadTilesForGpx(const GpxItem &gpxItem)
{
    AreaGeometry::LineBufferGeometry lineGeometry;

    for(const GpxManager::Point & point : gpxItem.pointVector)
    {
        lineGeometry.line.push_back(QPointF(point.lon, point.lat));
    }

    lineGeometry.bufferWidth = 1.;

    MainWindow * mainWindow = MainWindow::getInstance();

    if(mainWindow != nullptr)
    {
        OSMDownloadAreaDialog::Setup setup;

        setup.geometry.geometryType = AreaGeometry::Type::Line;
        setup.geometry.geometry = QVariant::fromValue(lineGeometry);

        setup.levelFrom = 0;
        setup.levelTo = ZoomInfo::MaxZoomLevel;

        QFileInfo fi(gpxItem.filePath);

        mainWindow->showDownloadAreaDialog(setup, fi.fileName());
    }
}

bool GpxManager::isAllLoaaded() const
{
    return _isAllLoaded;
}

void GpxManager::cancelLoadGpx()
{
    _cancelLoad = true;
}
