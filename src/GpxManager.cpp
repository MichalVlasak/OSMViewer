#include "GpxManager.h"
#include "AppSettings.h"

#include <QFileInfo>

GpxManager::GpxManager()
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
}

bool GpxManager::restoreConfig(QDomDocument &document)
{
    bool result = false;

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
        }
    }

    return result;
}

const QString & GpxManager::getLastPathToGpxFiles() const
{
    return _lastPathToGpxFiles;
}

void GpxManager::loadGpxFiles(const QStringList &fileNames)
{
    for(const QString & fileName : fileNames)
    {
        QFileInfo fileInfo(fileName);
        _lastPathToGpxFiles = fileInfo.dir().path();
    }
}
