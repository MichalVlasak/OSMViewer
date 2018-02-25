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

void AppSettings::storeConfig(StoreConfigInterface *obj)
{
    if(obj != nullptr)
    {
        obj->storeConfig(_doc, _rootElement);
    }
}

bool AppSettings::restoreConfig(StoreConfigInterface * obj)
{
    bool result = true;
    QFile file(_settingsFileName);

    QDomDocument document;
    if (document.setContent(&file) == false)
    {
        return false;
    }

    if(obj != nullptr)
    {
        result = obj->restoreConfig(document);
    }

    return result;
}
