#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include "Layers/MapSettings.h"
#include "StoreConfigInterface.h"

#include <QtXml>

class AppSettings
{
    public:
        AppSettings();
        ~AppSettings();

        static QString getValueString(QDomNode & node, const QString & name);

        void storeConfig(StoreConfigInterface * obj);
        bool restoreConfig(StoreConfigInterface * obj);

    private:
        QString _settingsFileName;
        QDomDocument _doc;
        QDomElement _rootElement;
};

#endif // APPSETTINGS_H
