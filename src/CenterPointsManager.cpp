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

const CenterPointsManager::CenterPointsVector & CenterPointsManager::getCenterPointsVector() const
{
    return _pointsVector;
}

bool compare(const CenterPointStruct & p1, const CenterPointStruct & p2)
{
    return (p1.name.compare(p2.name) < 0) ? true : false;
}

void CenterPointsManager::addCenterPoint(const CenterPointStruct &centerPoint)
{
    bool isPointPresent = false;

    for(const CenterPointStruct & point : _pointsVector)
    {
        if(point.name.compare(centerPoint.name) == 0)
        {
            isPointPresent = true;
            break;
        }
    }

    if(isPointPresent == false)
    {
        _pointsVector.push_back(centerPoint);

        std::sort(_pointsVector.begin(), _pointsVector.end(), compare);

        emit pointsWasAdded();
    }
}

void CenterPointsManager::removeCenterPoint(const CenterPointStruct &centerPoint)
{
    bool isPointPresent = false;
    size_t i = 0;

    for(i = 0; i < _pointsVector.size(); i++)
    {
        if(_pointsVector[i].name.compare(centerPoint.name) == 0)
        {
            isPointPresent = true;
            break;
        }
    }

    if(isPointPresent == true)
    {
        _pointsVector.erase(_pointsVector.begin() + i);

        std::sort(_pointsVector.begin(), _pointsVector.end(), compare);

        emit pointsWasRemoved();
    }
}

void CenterPointsManager::removeAllCenterPoints()
{
    _pointsVector.clear();

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

                QDomNodeList pointsNodeList = centerPointsNode.toElement().elementsByTagName("Points");

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

                                    addCenterPoint(point);
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

void CenterPointsManager::exportPoints()
{
    QWidget * widget = qobject_cast<QWidget*>(parent());

    QString fileName = QFileDialog::getSaveFileName(widget, tr("Export Center Points"), _importExportLastPath, tr("XML (*.xml)"));

    if(fileName.isEmpty() == true)
    {
        return;
    }

    QDomDocument doc;
    QDomElement _rootElement;
    QDomNode xmlNode = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");

    doc.insertBefore(xmlNode, doc.firstChild());

    _rootElement = doc.createElement("OSMViewerCenterPoints");

    doc.appendChild(_rootElement);

    QDomElement centerPointsElement = doc.createElement("CenterPoints");
    _rootElement.appendChild(centerPointsElement);

    QDomElement pointsListElement = doc.createElement("Points");
    centerPointsElement.appendChild(pointsListElement);

    for(const CenterPointStruct & point : _pointsVector)
    {
        QDomElement pointElement = doc.createElement("Point");
        pointsListElement.appendChild(pointElement);

        storePoint(point, pointElement, doc);
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
