#include "CenterPointsManager.h"
#include "AppSettings.h"
#include "MainWindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>

CenterPointsManager::CenterPointsManager(QObject * parent)
    : QObject(parent),
      _homePosition(CenterPointStruct(QPointF(0.0, 0.0), "Zero Point", -1))
{
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

void CenterPointsManager::importXml(const QString & fileName)
{
    QDomDocument document;
    QFile file(fileName);
    QFileInfo fileInfo(fileName);

    if (document.setContent(&file) == false)
    {
        QMessageBox::warning(MainWindow::getInstance(), tr("XML Error"), tr("Cannot read XML file \"") + fileName + tr("\"!"));
        return;
    }

    _importExportLastPath = fileInfo.dir().path();
    _importLastSuffix = fileInfo.suffix();

    QDomElement rootElem = document.firstChildElement("OSMViewerCenterPoints");

    QProgressDialog progressDialog(tr("Import XML \"") + fileName + tr("\" ..."), tr("Abort import"), 0, 100, MainWindow::getInstance());
    progressDialog.setWindowModality(Qt::WindowModal);

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

                                CenterPointsMap::iterator it = _pointsMap.find(groupName);

                                if(it == _pointsMap.end())
                                {
                                    CenterPointsVector points;
                                    _pointsMap[groupName] = points;
                                    it = _pointsMap.find(groupName);
                                }

                                QDomNodeList pointsNodeList = groupNode.toElement().elementsByTagName("Points");

                                for(int iPoints = 0; iPoints < pointsNodeList.size(); iPoints++)
                                {
                                    QDomNode pointsNode = pointsNodeList.at(iPoints);

                                    if(pointsNode.isNull() == false)
                                    {
                                        QDomNodeList pointNode = pointsNode.toElement().elementsByTagName("Point");
                                        int pointNodeCount = pointNode.size();

                                        for(int iPoint = 0; iPoint < pointNodeCount; iPoint++)
                                        {
                                            float percentage = (static_cast<float>(iPoint) / static_cast<float>(pointNodeCount)) * 100.;

                                            progressDialog.setValue(static_cast<int>(percentage));

                                            if(progressDialog.wasCanceled() == true)
                                            {
                                                break;
                                            }

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

                                                    if(it != _pointsMap.end())
                                                    {
                                                        CenterPointsVector & points = it->second;
                                                        points.push_back(point);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                                if(it != _pointsMap.end())
                                {
                                    CenterPointsVector & points = it->second;
                                    std::sort(points.begin(), points.end(), compare);

                                    emit pointsWasAdded();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void CenterPointsManager::importCsv(const QString &fileName)
{
    QFile file(fileName);

    if(file.open(QIODevice::ReadOnly) == true)
    {
        QFileInfo fileInfo(fileName);

        _importExportLastPath = fileInfo.dir().path();
        _importLastSuffix = fileInfo.suffix();

        QTextStream in(&file);
        int colCount = -1;
        int colCityName = -1;
        int colLat = -1;
        int colLng = -1;
        int colCountry = -1;
        int colPopulation = -1;

        auto trimedItem = [](QString item)
        {
            item = item.trimmed();

            while(item.indexOf("\"") == 0)
            {
                item = item.remove(0, 1);
            }

            if(item.size() > 0)
            {
                while(item.indexOf("\"") == item.size() - 1)
                {
                    item = item.remove(item.size() - 1, 1);
                }
            }

            return item;
        };

        if(!in.atEnd())
        {
            QString firstLine = in.readLine();
            QStringList firstLineList = firstLine.split(",");

            if(firstLineList.size() > 0)
            {
                colCount = firstLineList.size();

                for(int i = 0; i < colCount; i++)
                {
                    QString headerItem = trimedItem(firstLineList.at(i));

                    if(headerItem.compare("city_ascii", Qt::CaseInsensitive) == 0)
                    {
                        colCityName = i;
                    }
                    else if(headerItem.compare("lat", Qt::CaseInsensitive) == 0)
                    {
                        colLat = i;
                    }
                    else if(headerItem.compare("lng", Qt::CaseInsensitive) == 0)
                    {
                        colLng = i;
                    }
                    else if(headerItem.compare("country", Qt::CaseInsensitive) == 0)
                    {
                        colCountry = i;
                    }
                    else if(headerItem.compare("population", Qt::CaseInsensitive) == 0)
                    {
                        colPopulation = i;
                    }
                }
            }
        }

        int maxIndex = colCityName;

        maxIndex = std::max(maxIndex, colLat);
        maxIndex = std::max(maxIndex, colLng);
        maxIndex = std::max(maxIndex, colCountry);
        maxIndex = std::max(maxIndex, colPopulation);

        qint64 fileSize = file.size();
        float prevPercentage = 0.;
        QString groupName = fileInfo.baseName();

        CenterPointsMap::iterator it = _pointsMap.find(groupName);

        if(it == _pointsMap.end())
        {
            CenterPointsVector points;
            _pointsMap[groupName] = points;
            it = _pointsMap.find(groupName);
        }

        QProgressDialog progressDialog(tr("Import CSV \"") + fileName + tr("\" ..."), tr("Abort import"), 0, 100, MainWindow::getInstance());
        progressDialog.setWindowModality(Qt::WindowModal);

        while (!in.atEnd())
        {
            qint64 pos = in.pos();
            float percentage = (static_cast<float>(pos) / static_cast<float>(fileSize)) * 100.;

            if(percentage > (prevPercentage + 0.1))
            {
                progressDialog.setValue(static_cast<int>(percentage));

                if(progressDialog.wasCanceled() == true)
                {
                    break;
                }

                prevPercentage = percentage;
            }

            QString line = in.readLine();
            QStringList lineList = line.split(",");

            if(maxIndex <= lineList.size())
            {
                QString cityName = trimedItem(lineList.at(colCityName));
                QString lat = trimedItem(lineList.at(colLat));
                QString lon = trimedItem(lineList.at(colLng));
                QString country = trimedItem(lineList.at(colCountry));
                QString population = trimedItem(lineList.at(colPopulation));

                if(cityName.isEmpty() == false && lat.isEmpty() == false && lon.isEmpty() == false)
                {
                    QString name = cityName;

                    if(country.isEmpty() == false)
                    {
                        name += " (" + country + ")";
                    }

                    int level = 10;

                    if(population.isEmpty() == false)
                    {
                        bool isOk = false;
                        float populationFloat = population.toFloat(&isOk);

                        if(isOk == true)
                        {
                            if(populationFloat < 50000)
                            {
                                level = 16;
                            }
                            if(populationFloat < 100000)
                            {
                                level = 15;
                            }
                            else if(populationFloat < 300000)
                            {
                                level = 14;
                            }
                            else if(populationFloat < 1000000)
                            {
                                level = 13;
                            }
                            else if(populationFloat < 5000000)
                            {
                                level = 12;
                            }
                            else if(populationFloat < 10000000)
                            {
                                level = 11;
                            }
                        }
                    }

                    CenterPointStruct point;

                    point.name = name;
                    point.level = level;
                    point.position = QPointF(lon.toDouble(), lat.toDouble());

                    if(it != _pointsMap.end())
                    {
                        CenterPointsVector & points = it->second;
                        points.push_back(point);
                    }
                }
            }
        }

        if(it != _pointsMap.end())
        {
            CenterPointsVector & points = it->second;
            std::sort(points.begin(), points.end(), compare);

            emit pointsWasAdded();
        }

        file.close();
    }
}

void CenterPointsManager::importPoints()
{
    QWidget * widget = qobject_cast<QWidget*>(parent());
    QString xmlFilter = tr("XML (*.xml)");
    QString csvFilter = tr("CSV (*.csv)"); //(from https://simplemaps.com/data/world-cities)
    QString selectedFilter = xmlFilter;

    if(_importLastSuffix.compare("XML", Qt::CaseInsensitive) == 0)
    {
        selectedFilter = xmlFilter;
    }
    else if(_importLastSuffix.compare("CSV", Qt::CaseInsensitive) == 0)
    {
        selectedFilter = csvFilter;
    }

    QString fileName = QFileDialog::getOpenFileName(widget, tr("Import Center Points"), _importExportLastPath, xmlFilter + ";;" + csvFilter, &selectedFilter);

    if(fileName.isEmpty() == true)
    {
        return;
    }

    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix();

    if(suffix.compare("XML", Qt::CaseInsensitive) == 0)
    {
        importXml(fileName);
    }
    else if(suffix.compare("CSV", Qt::CaseInsensitive) == 0)
    {
        importCsv(fileName);
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
    QDomText importExportPathText = document.createTextNode(_importExportLastPath);
    importExportPathElement.appendChild(importExportPathText);

    QDomElement importSuffixPathElement = document.createElement("LastSuffixForImport");
    centerPointsElement.appendChild(importSuffixPathElement);
    QDomText importSuffixPathText = document.createTextNode(_importLastSuffix);
    importSuffixPathElement.appendChild(importSuffixPathText);

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
                    _importExportLastPath = path;

                    result = true;
                }

                QString suffix = AppSettings::getValueString(centerPointsNode, "LastSuffixForImport");

                if(suffix.isEmpty() == false)
                {
                    _importLastSuffix = suffix;

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
