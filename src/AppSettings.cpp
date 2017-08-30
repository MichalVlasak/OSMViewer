#include "AppSettings.h"
#include "MainWindow.h"
#include "Layers/OSMLayer.h"
#include "Layers/DownloadAreaHighlight.h"

#include <QTextStream>
#include <QDebug>

#include <iostream>

AppSettings::AppSettings()
    : _settingsFileName("OSMViewerSettings.xml")
{
    QDomNode xmlNode = _doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");

    _doc.insertBefore(xmlNode, _doc.firstChild());

    _rootElement = _doc.createElement("OSMViewer");

    _doc.appendChild(_rootElement);
}

AppSettings::~AppSettings()
{
    QFile file(_settingsFileName);

    if(file.open(QIODevice::WriteOnly) == true)
    {
        QTextStream out(&file);
        _doc.save(out, 4);
        file.close();
    }
}

QString getValueString(QDomNode node, QString name)
{
    QDomNodeList readNodes = node.toElement().elementsByTagName(name);

    for(int i = 0; i < readNodes.size() ; i++)
    {
        QDomNode readNode = readNodes.at(i);

        if(readNode.isNull() == false)
        {
            return readNode.toElement().text();
        }
    }

    return QString();
}

bool AppSettings::restoreMapSettings(MapSettings &mapSettings)
{
    bool result = false;
    QFile file(_settingsFileName);

    QDomDocument document;
    if (document.setContent(&file) == false)
    {
        /*std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << std::endl;*/
        return false;
    }

    QDomElement rootElem = document.firstChildElement("OSMViewer");

    if(rootElem.isNull() == false)
    {
        QDomNodeList mapNodes = rootElem.elementsByTagName("MapSettings");

        for(int iMap = 0; iMap < mapNodes.size(); iMap++)
        {
            QDomNode mapNode = mapNodes.at(iMap);

            if(mapNode.isNull() == false)
            {
                QString value = getValueString(mapNode, "ZoomLevel");

                if(value.isEmpty() == false)
                {
                    mapSettings.zoom.setCurrentZoomLevel(value.toUInt());
                    result = true;
                }

                value = getValueString(mapNode, "CenterX");

                if(value.isEmpty() == false)
                {
                    mapSettings.worldCenter.rx() = value.toFloat();
                    result = true;
                }

                value = getValueString(mapNode, "CenterY");

                if(value.isEmpty() == false)
                {
                    mapSettings.worldCenter.ry() = value.toFloat();
                    result = true;
                }
            }
        }
    }

    return result;
}

void AppSettings::storeMapSettings(MapSettings & mapSettings)
{
    QDomElement mapElement = _doc.createElement("MapSettings");
    _rootElement.appendChild(mapElement);

    QDomElement zoomElement = _doc.createElement("ZoomLevel");
    mapElement.appendChild(zoomElement);
    QDomText zoomText = _doc.createTextNode(QString::number(mapSettings.zoom.getCurrentZoomLevel()));
    zoomElement.appendChild(zoomText);

    QDomElement centerXElement = _doc.createElement("CenterX");
    mapElement.appendChild(centerXElement);
    QDomText centerXText = _doc.createTextNode(QString::number(mapSettings.worldCenter.x()));
    centerXElement.appendChild(centerXText);

    QDomElement centerYElement = _doc.createElement("CenterY");
    mapElement.appendChild(centerYElement);
    QDomText centerYText = _doc.createTextNode(QString::number(mapSettings.worldCenter.y()));
    centerYElement.appendChild(centerYText);
}

void AppSettings::storeMainWindowSettings(MainWindow *mainWindow)
{
    QDomElement mainWindowElement = _doc.createElement("MainWindowSettings");
    _rootElement.appendChild(mainWindowElement);

    QByteArray save = mainWindow->saveState().toHex();
    std::string buffState(save.data(), save.size());
    QDomElement windowStateElement = _doc.createElement("WindowState");
    mainWindowElement.appendChild(windowStateElement);
    QDomText windowStateText = _doc.createTextNode(QString::fromStdString(buffState));
    windowStateElement.appendChild(windowStateText);

    save = mainWindow->saveGeometry().toHex();
    std::string buffGeometry(save.data(), save.size());
    QDomElement windowGeometryElement = _doc.createElement("WindowGeometry");
    mainWindowElement.appendChild(windowGeometryElement);
    QDomText windowGeometryText = _doc.createTextNode(QString::fromStdString(buffGeometry));
    windowGeometryElement.appendChild(windowGeometryText);
}

bool AppSettings::restoreMainWindowSettings(MainWindow *mainWindow)
{
    bool result = true;
    QFile file(_settingsFileName);

    QDomDocument document;
    if (document.setContent(&file) == false)
    {
        /*std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << std::endl;*/
        return false;
    }

    QDomElement rootElem = document.firstChildElement("OSMViewer");

    if(rootElem.isNull() == false)
    {
        QDomNodeList mainWindowNodes = rootElem.elementsByTagName("MainWindowSettings");

        for(int iMap = 0; iMap < mainWindowNodes.size(); iMap++)
        {
            QDomNode mapNode = mainWindowNodes.at(iMap);

            if(mapNode.isNull() == false)
            {
                QString value = getValueString(mapNode, "WindowState");

                if(value.isEmpty() == false)
                {
                    QByteArray state = QByteArray(value.toStdString().data(), value.size());
                    state = QByteArray::fromHex(state);
                    if(mainWindow->restoreState(state) == false)
                    {
                        std::cerr << "Cannot restore Main Window state!" << std::endl;
                        result |= false;
                    }
                }

                value = getValueString(mapNode, "WindowGeometry");

                if(value.isEmpty() == false)
                {
                    QByteArray state = QByteArray(value.toStdString().data(), value.size());
                    state = QByteArray::fromHex(state);
                    if(mainWindow->restoreGeometry(state) == false)
                    {
                        std::cerr << "Cannot restore Main Window geometry!" << std::endl;
                        result |= false;
                    }
                }
            }
        }
    }

    return result;
}

void AppSettings::storeOSMDirectoryPath(OSMLayer * layer)
{
    QDomElement layerElement = _doc.createElement("OSMLayerSettings");
    _rootElement.appendChild(layerElement);

    QDomElement directoryPathElement = _doc.createElement("OSMDirectoryPath");
    layerElement.appendChild(directoryPathElement);
    QDomText direcotryPathText = _doc.createTextNode(layer->getOSMDirectorypath());
    directoryPathElement.appendChild(direcotryPathText);
}

bool AppSettings::restoreOSMDirectoryPath(OSMLayer * layer)
{
    bool result = false;
    QFile file(_settingsFileName);

    QDomDocument document;
    if (document.setContent(&file) == false)
    {
        /*std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << std::endl;*/
        return false;
    }

    QDomElement rootElem = document.firstChildElement("OSMViewer");

    if(rootElem.isNull() == false)
    {
        QDomNodeList osmLayerNodes = rootElem.elementsByTagName("OSMLayerSettings");

        for(int iMap = 0; iMap < osmLayerNodes.size(); iMap++)
        {
            QDomNode layerNode = osmLayerNodes.at(iMap);

            if(layerNode.isNull() == false)
            {
                QString value = getValueString(layerNode, "OSMDirectoryPath");

                if(value.isEmpty() == false)
                {
                    if(layer != nullptr)
                    {
                        layer->setOSMDirectorypath(value);
                        result = true;
                    }
                }
            }
        }
    }

    return result;
}

void AppSettings::storeDownloadSettings(OSMTileDownloader * downloader)
{
    QDomElement downloaderElement = _doc.createElement("OSMDownloaderSettings");
    _rootElement.appendChild(downloaderElement);

    QDomElement downloadEnableElement = _doc.createElement("EnabledDownloading");
    downloaderElement.appendChild(downloadEnableElement);
    QDomText downloadEnableText = _doc.createTextNode(QString::number(downloader->isDownloadingEnable()));
    downloadEnableElement.appendChild(downloadEnableText);

    QDomElement threadsCountElement = _doc.createElement("ThreadsCount");
    downloaderElement.appendChild(threadsCountElement);
    QDomText threadsCountText = _doc.createTextNode(QString::number(downloader->getThreads()));
    threadsCountElement.appendChild(threadsCountText);
}

bool AppSettings::restoreDownloadSettings(OSMTileDownloader *downloader)
{
    bool result = false;
    QFile file(_settingsFileName);

    QDomDocument document;
    if (document.setContent(&file) == false)
    {
        /*std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << std::endl;*/
        return false;
    }

    QDomElement rootElem = document.firstChildElement("OSMViewer");

    if(rootElem.isNull() == false)
    {
        QDomNodeList downloadSettingsNodes = rootElem.elementsByTagName("OSMDownloaderSettings");

        for(int iMap = 0; iMap < downloadSettingsNodes.size(); iMap++)
        {
            QDomNode downloadSettingNode = downloadSettingsNodes.at(iMap);

            if(downloadSettingNode.isNull() == false)
            {
                QString value = getValueString(downloadSettingNode, "EnabledDownloading");

                if(value.isEmpty() == false)
                {
                    if(downloader != nullptr)
                    {
                        downloader->setDownloadingEnable(value.toInt());
                        result = true;
                    }
                }

                value = getValueString(downloadSettingNode, "ThreadsCount");

                if(value.isEmpty() == false)
                {
                    if(downloader != nullptr)
                    {
                        downloader->setThreads(value.toInt());
                        result = true;
                    }
                }
            }
        }
    }

    return result;
}

void AppSettings::storeDownloadAreaHighlightSettings(DownloadAreaHighlight * highlighter)
{
    QDomElement downloaderElement = _doc.createElement("DownloadAreaHighlight");
    _rootElement.appendChild(downloaderElement);

    QDomElement downloadEnableElement = _doc.createElement("VisibleDownloadAreaHighlight");
    downloaderElement.appendChild(downloadEnableElement);
    QDomText downloadEnableText = _doc.createTextNode(QString::number(highlighter->isVisible()));
    downloadEnableElement.appendChild(downloadEnableText);
}

bool AppSettings::restoreDownloadAreaHighlightSettings(DownloadAreaHighlight *highlighter)
{
    bool result = false;
    QFile file(_settingsFileName);

    QDomDocument document;
    if (document.setContent(&file) == false)
    {
        /*std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << std::endl;*/
        return false;
    }

    QDomElement rootElem = document.firstChildElement("OSMViewer");

    if(rootElem.isNull() == false)
    {
        QDomNodeList downloadSettingsNodes = rootElem.elementsByTagName("DownloadAreaHighlight");

        for(int iMap = 0; iMap < downloadSettingsNodes.size(); iMap++)
        {
            QDomNode downloadSettingNode = downloadSettingsNodes.at(iMap);

            if(downloadSettingNode.isNull() == false)
            {
                QString value = getValueString(downloadSettingNode, "VisibleDownloadAreaHighlight");

                if(value.isEmpty() == false)
                {
                    if(highlighter != nullptr)
                    {
                        int isVisible = value.toInt();
                        highlighter->setVisible((isVisible > 0) ? true : false);
                        result = true;
                    }
                }
            }
        }
    }

    return result;
}
