#include "CenterPointsManager.h"
#include "AppSettings.h"

#include <QFileDialog>

CenterPointsManager::CenterPointsManager(QObject * parent)
    : QObject(parent)
{
    _homePosition = CenterPointStruct(QPointF(0.0, 0.0), "Zero Point", -1);
    _importExportLastPath = ".";

//    addCenterPoint(CenterPointStruct(QPointF(17.1097222222, 48.1438888889), "Bratislava", 12));
//    addCenterPoint(CenterPointStruct(QPointF(14.4211111111, 50.0872222222), "Praha"     , 12));
//    addCenterPoint(CenterPointStruct(QPointF(16.3730555556, 48.2083333333), "Vieden"    , 12));
//    addCenterPoint(CenterPointStruct(QPointF(19.0502777778, 47.4719444444), "Budapest"  , 12));
//    addCenterPoint(CenterPointStruct(QPointF(21.0083333333, 52.2322222222), "Varsava"   , 12));
//    addCenterPoint(CenterPointStruct(QPointF(13.4         , 52.5166666667), "Berlin"    , 11));
}

const CenterPointStruct & CenterPointsManager::getHomeCenterPoint() const
{
    return _homePosition;
}

void CenterPointsManager::setHomeCenterPoint(const CenterPointStruct & point)
{
    _homePosition = point;

    emit homePointWasChanged();
}

const CenterPointsManager::CenterPointsVector * CenterPointsManager::getCenterPointsVector(const QString & groupName) const
{
    CenterPointsMap::const_iterator it = _pointsMap.find(groupName);

    if(it != _pointsMap.cend())
    {
        return &(it->second);
    }

    return nullptr;
}

const CenterPointsManager::CenterPointsMap & CenterPointsManager::getCenterPointsMap() const
{
    return _pointsMap;
}

bool compare(const CenterPointStruct & p1, const CenterPointStruct & p2)
{
    return (p1.name.compare(p2.name) < 0) ? true : false;
}

void CenterPointsManager::addCenterPoint(const QString & groupName, const CenterPointStruct &centerPoint, bool emitSignal)
{
    CenterPointsMap::iterator it = _pointsMap.find(groupName);

    if(it == _pointsMap.end())
    {
        CenterPointsVector points;
        _pointsMap[groupName] = points;
        it = _pointsMap.find(groupName);
    }

    if(it != _pointsMap.end())
    {
        CenterPointsVector & points = it->second;
        bool isPointPresent = false;

        for(const CenterPointStruct & point : points)
        {
            if(point.name.compare(centerPoint.name) == 0)
            {
                isPointPresent = true;
                break;
            }
        }

        if(isPointPresent == false)
        {
            points.push_back(centerPoint);

            std::sort(points.begin(), points.end(), compare);

            if(emitSignal == true)
            {
                emit pointsWasAdded();
            }
        }
    }
}

void CenterPointsManager::removeCenterPoint(const QString & groupName, const CenterPointStruct &centerPoint, bool emitSignal)
{
    CenterPointsMap::iterator it = _pointsMap.find(groupName);

    if(it != _pointsMap.end())
    {
        CenterPointsVector & points = it->second;
        bool isPointPresent = false;
        size_t i = 0;

        for(i = 0; i < points.size(); i++)
        {
            if(points[i].name.compare(centerPoint.name) == 0)
            {
                isPointPresent = true;
                break;
            }
        }

        if(isPointPresent == true)
        {
            points.erase(points.begin() + i);

            std::sort(points.begin(), points.end(), compare);

            if(emitSignal == true)
            {
                emit pointsWasRemoved();
            }
        }
    }
}

void CenterPointsManager::removeAllCenterPoints()
{
    _pointsMap.clear();

    emit pointsWasRemoved();
}

void CenterPointsManager::importPoints()
{
    QWidget * widget = qobject_cast<QWidget*>(parent());

    QString fileName = QFileDialog::getOpenFileName(widget, tr("Import Center Points"), _importExportLastPath, tr("XML (*.xml)"));

    if(fileName.isEmpty() == true)
    {
        return;
    }

    QFile file(fileName);

    QDomDocument document;

    if (document.setContent(&file) == false)
    {
        /*std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << std::endl;*/
        return;
    }

    QFileInfo fileInfo(fileName);
    _importExportLastPath = fileInfo.dir().path();

    QDomElement rootElem = document.firstChildElement("OSMViewerCenterPoints");

    if(rootElem.isNull() == false)
    {
        QDomNodeList centerPointsNodes = rootElem.elementsByTagName("CenterPoints");

        for(int iPointsMain = 0; iPointsMain < centerPointsNodes.size(); iPointsMain++)
        {
            QDomNode centerPointsNode = centerPointsNodes.at(iPointsMain);

            if(centerPointsNode.isNull() == false)
            {
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
                                                QString name = AppSettings::getValueString(pPointNode, "Name");
                                                QString level = AppSettings::getValueString(pPointNode, "Level");
                                                QString lat = AppSettings::getValueString(pPointNode, "Latitude");
                                                QString lon = AppSettings::getValueString(pPointNode, "Longitude");

                                                if(name.isEmpty() == false && level.isEmpty() == false && lat.isEmpty() == false && lon.isEmpty() == false)
                                                {
                                                    CenterPointStruct point;

                                                    point.name = name;
                                                    point.level = level.toInt();
                                                    point.position = QPointF(lon.toDouble(), lat.toDouble());

                                                    addCenterPoint(groupName, point, false);
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
}

void CenterPointsManager::storePoint(const CenterPointStruct & point, QDomElement & element, QDomDocument & doc)
{
    QDomElement homePointNameElement = doc.createElement("Name");
    element.appendChild(homePointNameElement);
    QDomText homePointNameText = doc.createTextNode(point.name);
    homePointNameElement.appendChild(homePointNameText);

    QDomElement homePointLevelElement = doc.createElement("Level");
    element.appendChild(homePointLevelElement);
    QDomText homePointLevelText = doc.createTextNode(QString::number(point.level));
    homePointLevelElement.appendChild(homePointLevelText);

    QDomElement homePointLatitudeElement = doc.createElement("Latitude");
    element.appendChild(homePointLatitudeElement);
    QDomText homePointLatitudeText = doc.createTextNode(QString::number(point.position.y(), 'g', 13));
    homePointLatitudeElement.appendChild(homePointLatitudeText);

    QDomElement homePointLongitudeElement = doc.createElement("Longitude");
    element.appendChild(homePointLongitudeElement);
    QDomText homePointLongitudeText = doc.createTextNode(QString::number(point.position.x(), 'g', 13));
    homePointLongitudeElement.appendChild(homePointLongitudeText);
}

void CenterPointsManager::prepareXmlDocument(const QString & fileName, QDomDocument & doc, QDomElement & groupsElement)
{
    QDomElement rootElement;
    QDomNode xmlNode = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");

    doc.insertBefore(xmlNode, doc.firstChild());

    rootElement = doc.createElement("OSMViewerCenterPoints");

    doc.appendChild(rootElement);

    QDomElement centerPointsElement = doc.createElement("CenterPoints");
    rootElement.appendChild(centerPointsElement);

    groupsElement = doc.createElement("Groups");
    centerPointsElement.appendChild(groupsElement);
}

void CenterPointsManager::savePointsToXml(CenterPointsMap::iterator it, QDomDocument & doc, QDomElement & groupsElement)
{
    QDomElement groupElement = doc.createElement("Group");
    groupsElement.appendChild(groupElement);

    QDomElement groupNameElement = doc.createElement("Name");
    groupElement.appendChild(groupNameElement);
    QDomText groupPointNameText = doc.createTextNode(it->first);
    groupNameElement.appendChild(groupPointNameText);

    QDomElement pointsListElement = doc.createElement("Points");
    groupElement.appendChild(pointsListElement);

    for(const CenterPointStruct & point : it->second)
    {
        QDomElement pointElement = doc.createElement("Point");
        pointsListElement.appendChild(pointElement);

        storePoint(point, pointElement, doc);
    }
}

void CenterPointsManager::exportAllGroups()
{
    QWidget * widget = qobject_cast<QWidget*>(parent());

    QString fileName = QFileDialog::getSaveFileName(widget, tr("Export Center Points"), _importExportLastPath, tr("XML (*.xml)"));

    if(fileName.isEmpty() == true)
    {
        return;
    }

    QDomDocument doc;
    QDomElement groupsElement;

    prepareXmlDocument(fileName, doc, groupsElement);

    CenterPointsMap::iterator it, end = _pointsMap.end();

    for(it = _pointsMap.begin(); it != end; ++it)
    {
        savePointsToXml(it, doc, groupsElement);
    }

    QFile file(fileName);

    if(file.open(QIODevice::WriteOnly) == true)
    {
        QTextStream out(&file);
        doc.save(out, 4);
        file.close();

        QFileInfo fileInfo(fileName);
        _importExportLastPath = fileInfo.dir().path();
    }
}

void CenterPointsManager::exportGroup(const QString &groupName)
{
    QWidget * widget = qobject_cast<QWidget*>(parent());

    QString fileName = QFileDialog::getSaveFileName(widget, tr("Export Center Points"), _importExportLastPath, tr("XML (*.xml)"));

    if(fileName.isEmpty() == true)
    {
        return;
    }

    QDomDocument doc;
    QDomElement groupsElement;

    prepareXmlDocument(fileName, doc, groupsElement);

    CenterPointsMap::iterator it = _pointsMap.find(groupName);

    if(it != _pointsMap.end())
    {
        savePointsToXml(it, doc, groupsElement);
    }

    QFile file(fileName);

    if(file.open(QIODevice::WriteOnly) == true)
    {
        QTextStream out(&file);
        doc.save(out, 4);
        file.close();

        QFileInfo fileInfo(fileName);
        _importExportLastPath = fileInfo.dir().path();
    }
}

const QString & CenterPointsManager::getImportExportLastPath() const
{
    return _importExportLastPath;
}

void CenterPointsManager::setImportExportLastPath(const QString &path)
{
    _importExportLastPath = path;
}

bool CenterPointsManager::createNewGroup(const QString &groupName)
{
    CenterPointsMap::iterator it = _pointsMap.find(groupName);

    if(it == _pointsMap.end())
    {
        CenterPointsVector points;

        _pointsMap[groupName] = points;

        return true;
    }

    return false;
}

bool CenterPointsManager::changeGroupName(const QString & oldGroupName, const QString & newGroupName)
{
    CenterPointsMap::iterator itOld = _pointsMap.find(oldGroupName);

    if(itOld != _pointsMap.end())
    {
        CenterPointsMap::iterator itNew = _pointsMap.find(newGroupName);

        if(itNew == _pointsMap.end())
        {
            _pointsMap[newGroupName] = itOld->second;
            _pointsMap.erase(itOld);

            return true;
        }
    }

    return false;
}

bool CenterPointsManager::removeGroup(const QString &groupName)
{
    CenterPointsMap::iterator it = _pointsMap.find(groupName);

    if(it != _pointsMap.end())
    {
        _pointsMap.erase(it);

        return true;
    }

    return false;
}

void CenterPointsManager::storeConfig(QDomDocument &document, QDomElement &rootElement)
{
    CenterPointStruct homePoint = getHomeCenterPoint();

    QDomElement centerPointsElement = document.createElement("CenterPoints");
    rootElement.appendChild(centerPointsElement);

    QDomElement importExportPathElement = document.createElement("LastPathForImportExport");
    centerPointsElement.appendChild(importExportPathElement);
    QDomText importExportPathText = document.createTextNode(getImportExportLastPath());
    importExportPathElement.appendChild(importExportPathText);

    QDomElement homePointElement = document.createElement("HomePoint");
    centerPointsElement.appendChild(homePointElement);

    CenterPointsManager::storePoint(homePoint, homePointElement, document);

    QDomElement groupsElement = document.createElement("Groups");
    centerPointsElement.appendChild(groupsElement);

    const CenterPointsManager::CenterPointsMap & pointsMap = getCenterPointsMap();
    CenterPointsManager::CenterPointsMap::const_iterator it, end = pointsMap.end();

    for(it = pointsMap.begin(); it != end; ++it)
    {
        QDomElement groupElement = document.createElement("Group");
        groupsElement.appendChild(groupElement);

        QDomElement groupNameElement = document.createElement("Name");
        groupElement.appendChild(groupNameElement);
        QDomText groupPointNameText = document.createTextNode(it->first);
        groupNameElement.appendChild(groupPointNameText);

        QDomElement pointsListElement = document.createElement("Points");
        groupElement.appendChild(pointsListElement);

        const CenterPointsManager::CenterPointsVector & points = it->second;

        for(const CenterPointStruct & point : points)
        {
            QDomElement pointElement = document.createElement("Point");
            pointsListElement.appendChild(pointElement);

            CenterPointsManager::storePoint(point, pointElement, document);
        }
    }
}

bool CenterPointsManager::restoreConfig(QDomDocument &document)
{
    bool result = false;
    QDomElement rootElem = document.firstChildElement("OSMViewer");

    if(rootElem.isNull() == false)
    {
        QDomNodeList centerPointsNodes = rootElem.elementsByTagName("CenterPoints");

        for(int iPointsMain = 0; iPointsMain < centerPointsNodes.size(); iPointsMain++)
        {
            QDomNode centerPointsNode = centerPointsNodes.at(iPointsMain);

            if(centerPointsNode.isNull() == false)
            {
                QString path = AppSettings::getValueString(centerPointsNode, "LastPathForImportExport");

                if(path.isEmpty() == false)
                {
                    setImportExportLastPath(path);

                    result = true;
                }

                QDomNodeList homePointsNode = centerPointsNode.toElement().elementsByTagName("HomePoint");

                for(int iHomePoint = 0; iHomePoint < homePointsNode.size(); iHomePoint++)
                {
                    QDomNode homePointNode = homePointsNode.at(iHomePoint);

                    if(homePointNode.isNull() == false)
                    {
                        QString name = AppSettings::getValueString(homePointNode, "Name");
                        QString level = AppSettings::getValueString(homePointNode, "Level");
                        QString lat = AppSettings::getValueString(homePointNode, "Latitude");
                        QString lon = AppSettings::getValueString(homePointNode, "Longitude");

                        if(name.isEmpty() == false && level.isEmpty() == false && lat.isEmpty() == false && lon.isEmpty() == false)
                        {
                            CenterPointStruct homePoint;

                            homePoint.name = name;
                            homePoint.level = level.toInt();
                            homePoint.position = QPointF(lon.toDouble(), lat.toDouble());

                            setHomeCenterPoint(homePoint);

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
                                                QString name = AppSettings::getValueString(pPointNode, "Name");
                                                QString level = AppSettings::getValueString(pPointNode, "Level");
                                                QString lat = AppSettings::getValueString(pPointNode, "Latitude");
                                                QString lon = AppSettings::getValueString(pPointNode, "Longitude");

                                                if(name.isEmpty() == false && level.isEmpty() == false && lat.isEmpty() == false && lon.isEmpty() == false)
                                                {
                                                    CenterPointStruct point;

                                                    point.name = name;
                                                    point.level = level.toInt();
                                                    point.position = QPointF(lon.toDouble(), lat.toDouble());

                                                    addCenterPoint(groupName, point, false);

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
