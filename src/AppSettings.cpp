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

QString AppSettings::getValueString(QDomNode & node, const QString & name)
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

    QDomElement deleteTilesEnabled = _doc.createElement("DeleteTilesEnabled");
    mainWindowElement.appendChild(deleteTilesEnabled);
    QDomText deleteTilesEnabledText = _doc.createTextNode(QString::number(mainWindow->getDeleteSettings().deleteEnabled));
    deleteTilesEnabled.appendChild(deleteTilesEnabledText);

    QDomElement deleteTilesType = _doc.createElement("DeleteTilesType");
    mainWindowElement.appendChild(deleteTilesType);
    QDomText deleteTilesTypeText = _doc.createTextNode(QString::number(mainWindow->getDeleteSettings().deleteType));
    deleteTilesType.appendChild(deleteTilesTypeText);

    QDomElement deleteTilesTime = _doc.createElement("DeleteTilesTime");
    mainWindowElement.appendChild(deleteTilesTime);
    QDomText deleteTilesTimeText = _doc.createTextNode(mainWindow->getDeleteSettings().deleteTime.toString(Qt::DateFormat::ISODate));
    deleteTilesTime.appendChild(deleteTilesTimeText);
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

                DeleteOldMapsWidget::DeleteSettings deleteSettings;
                value = getValueString(mapNode, "DeleteTilesEnabled");

                if(value.isEmpty() == false)
                {
                    bool deleteTiles = (value.toInt() == 0) ? false : true;
                    deleteSettings.deleteEnabled = deleteTiles;
                }
                else
                {
                    deleteSettings.deleteEnabled = false;
                }

                value = getValueString(mapNode, "DeleteTilesType");

                if(value.isEmpty() == false)
                {
                    deleteSettings.deleteType = DeleteOldMapsWidget::DeleteType(value.toInt());
                }
                else
                {
                    deleteSettings.deleteType = DeleteOldMapsWidget::DeleteOldAsTime;
                }

                value = getValueString(mapNode, "DeleteTilesTime");

                if(value.isEmpty() == false)
                {
                    deleteSettings.deleteTime = QDateTime::fromString(value, Qt::DateFormat::ISODate);
                }
                else
                {
                    deleteSettings.deleteTime = QDateTime::currentDateTime();
                }

                mainWindow->setDeleteSettings(deleteSettings);
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

void AppSettings::storeCenterPoints(CenterPointsManager *pointsManager)
{
    if(pointsManager != nullptr)
    {
        CenterPointStruct homePoint = pointsManager->getHomeCenterPoint();

        QDomElement centerPointsElement = _doc.createElement("CenterPoints");
        _rootElement.appendChild(centerPointsElement);

        QDomElement importExportPathElement = _doc.createElement("LastPathForImportExport");
        centerPointsElement.appendChild(importExportPathElement);
        QDomText importExportPathText = _doc.createTextNode(pointsManager->getImportExportLastPath());
        importExportPathElement.appendChild(importExportPathText);

        QDomElement homePointElement = _doc.createElement("HomePoint");
        centerPointsElement.appendChild(homePointElement);

        CenterPointsManager::storePoint(homePoint, homePointElement, _doc);

        QDomElement groupsElement = _doc.createElement("Groups");
        centerPointsElement.appendChild(groupsElement);

        const CenterPointsManager::CenterPointsMap & pointsMap = pointsManager->getCenterPointsMap();
        CenterPointsManager::CenterPointsMap::const_iterator it, end = pointsMap.end();

        for(it = pointsMap.begin(); it != end; ++it)
        {
            QDomElement groupElement = _doc.createElement("Group");
            groupsElement.appendChild(groupElement);

            QDomElement groupNameElement = _doc.createElement("Name");
            groupElement.appendChild(groupNameElement);
            QDomText groupPointNameText = _doc.createTextNode(it->first);
            groupNameElement.appendChild(groupPointNameText);

            QDomElement pointsListElement = _doc.createElement("Points");
            groupElement.appendChild(pointsListElement);

            const CenterPointsManager::CenterPointsVector & points = it->second;

            for(const CenterPointStruct & point : points)
            {
                QDomElement pointElement = _doc.createElement("Point");
                pointsListElement.appendChild(pointElement);

                CenterPointsManager::storePoint(point, pointElement, _doc);
            }
        }
    }
}

bool AppSettings::restoreCenterPoints(CenterPointsManager *pointsManager)
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
        QDomNodeList centerPointsNodes = rootElem.elementsByTagName("CenterPoints");

        for(int iPointsMain = 0; iPointsMain < centerPointsNodes.size(); iPointsMain++)
        {
            QDomNode centerPointsNode = centerPointsNodes.at(iPointsMain);

            if(centerPointsNode.isNull() == false)
            {
                QString path = getValueString(centerPointsNode, "LastPathForImportExport");

                if(path.isEmpty() == false)
                {
                    pointsManager->setImportExportLastPath(path);

                    result = true;
                }

                QDomNodeList homePointsNode = centerPointsNode.toElement().elementsByTagName("HomePoint");

                for(int iHomePoint = 0; iHomePoint < homePointsNode.size(); iHomePoint++)
                {
                    QDomNode homePointNode = homePointsNode.at(iHomePoint);

                    if(homePointNode.isNull() == false)
                    {
                        QString name = getValueString(homePointNode, "Name");
                        QString level = getValueString(homePointNode, "Level");
                        QString lat = getValueString(homePointNode, "Latitude");
                        QString lon = getValueString(homePointNode, "Longitude");

                        if(name.isEmpty() == false && level.isEmpty() == false && lat.isEmpty() == false && lon.isEmpty() == false)
                        {
                            CenterPointStruct homePoint;

                            homePoint.name = name;
                            homePoint.level = level.toInt();
                            homePoint.position = QPointF(lon.toDouble(), lat.toDouble());

                            pointsManager->setHomeCenterPoint(homePoint);

                            result = true;
                        }
                    }
                }

                QDomNodeList groupsNodeList = centerPointsNode.toElement().elementsByTagName("Groups");

                for(int iGroups = 0; iGroups < groupsNodeList.size(); iGroups++)
                {
                    QDomNode groupsNode = groupsNodeList.at(iGroups);

                    if(groupsNode.isNull() == false)
                    {
                        QDomNodeList groupNodeList = groupsNode.toElement().elementsByTagName("Group");

                        for(int iGroup = 0; iGroup < groupNodeList.size(); iGroup++)
                        {
                            QDomNode groupNode = groupNodeList.at(iGroup);

                            if(groupNode.isNull() == false)
                            {
                                QString groupName = AppSettings::getValueString(groupNode, "Name");

                                QDomNodeList pointsNodeList = groupNode.toElement().elementsByTagName("Points");

                                for(int iPoints = 0; iPoints < pointsNodeList.size(); iPoints++)
                                {
                                    QDomNode pointsNode = pointsNodeList.at(iPoints);

                                    if(pointsNode.isNull() == false)
                                    {
                                        QDomNodeList pointNode = pointsNode.toElement().elementsByTagName("Point");

                                        for(int iPoint = 0; iPoint < pointNode.size(); iPoint++)
                                        {
                                            QDomNode pPointNode = pointNode.at(iPoint);

                                            if(pPointNode.isNull() == false)
                                            {
                                                QString name = getValueString(pPointNode, "Name");
                                                QString level = getValueString(pPointNode, "Level");
                                                QString lat = getValueString(pPointNode, "Latitude");
                                                QString lon = getValueString(pPointNode, "Longitude");

                                                if(name.isEmpty() == false && level.isEmpty() == false && lat.isEmpty() == false && lon.isEmpty() == false)
                                                {
                                                    CenterPointStruct point;

                                                    point.name = name;
                                                    point.level = level.toInt();
                                                    point.position = QPointF(lon.toDouble(), lat.toDouble());

                                                    pointsManager->addCenterPoint(groupName, point, false);

                                                    result = true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return result;
}

void AppSettings::storeProjects(OSMDownloadProjectModel *projectsModel)
{
    if(projectsModel != nullptr)
    {
        QDomElement projectsElement = _doc.createElement("Projects");
        _rootElement.appendChild(projectsElement);

        const OSMDownloadProjectModel::ProjectVector & projects = projectsModel->getProjects();

        for(const OSMDownloadProjectModel::Project & project : projects)
        {
            QDomElement projectElement = _doc.createElement("Project");
            projectsElement.appendChild(projectElement);

            OSMDownloadProjectModel::storeProject(project, projectElement, _doc);
        }
    }
}

bool AppSettings::restoreProjects(OSMDownloadProjectModel *projectsModel)
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
        QDomNodeList projectsNodes = rootElem.elementsByTagName("Projects");

        for(int iProjects = 0; iProjects < projectsNodes.size(); iProjects++)
        {
            QDomNode projectsNode = projectsNodes.at(iProjects);

            if(projectsNode.isNull() == false)
            {
                QDomNodeList projectNodeList = projectsNode.toElement().elementsByTagName("Project");

                for(int iProject = 0; iProject < projectNodeList.size(); iProject++)
                {
                    QDomNode projectNode = projectNodeList.at(iProject);

                    if(projectNode.isNull() == false)
                    {
                        QString projectName = AppSettings::getValueString(projectNode, "Name");
                        QString levelFrom = getValueString(projectNode, "LevelFrom");
                        QString levelTo = getValueString(projectNode, "LevelTo");
                        QString latFrom = getValueString(projectNode, "LatitudeFrom");
                        QString lonFrom = getValueString(projectNode, "LongitudeFrom");
                        QString latTo = getValueString(projectNode, "LatitudeTo");
                        QString lonTo = getValueString(projectNode, "LongitudeTo");

                        if(projectName.isEmpty() == false && levelFrom.isEmpty() == false && levelTo.isEmpty() == false &&
                           latFrom.isEmpty() == false && latTo.isEmpty() == false &&
                           lonFrom.isEmpty() == false && lonTo.isEmpty() == false)
                        {
                            OSMDownloadProjectModel::Project project;
                            bool isOK;

                            project.name = projectName;
                            project.setup.levelFrom = levelFrom.toDouble(&isOK);

                            if(isOK == false) break;

                            project.setup.levelTo = levelTo.toDouble(&isOK);

                            if(isOK == false) break;

                            project.setup.latFrom = latFrom.toDouble(&isOK);

                            if(isOK == false) break;

                            project.setup.latTo = latTo.toDouble(&isOK);

                            if(isOK == false) break;

                            project.setup.lonFrom = lonFrom.toDouble(&isOK);

                            if(isOK == false) break;

                            project.setup.lonTo = lonTo.toDouble(&isOK);

                            if(isOK == false) break;

                            if(projectsModel != nullptr)
                            {
                                result &= projectsModel->addProject(project);
                            }
                        }
                    }
                }
            }
        }
    }

    return result;
}
