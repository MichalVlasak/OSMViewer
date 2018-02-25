#ifndef STORECONFIGINTERFACE_H
#define STORECONFIGINTERFACE_H

#include <QtXml>

class StoreConfigInterface
{
    public:
        StoreConfigInterface();

    public:
        virtual void storeConfig(QDomDocument & document, QDomElement & rootElement) = 0;
        virtual bool restoreConfig(QDomDocument & document) = 0;
};

#endif // STORECONFIGINTERFACE_H
