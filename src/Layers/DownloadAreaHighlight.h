#ifndef DOWNLOADAREAHIGHLIGHT_H
#define DOWNLOADAREAHIGHLIGHT_H

#include "BaseLayer.h"
#include "../OSMDownloadAreaDialog.h"
#include "../StoreConfigInterface.h"

#include <QObject>

class DownloadAreaHighlight : public QObject, public BaseLayer, public StoreConfigInterface
{
        Q_OBJECT

    public:
        explicit DownloadAreaHighlight(const QString & layerName, MapSettings & mapSettings, QObject * parent = nullptr);
        virtual ~DownloadAreaHighlight() = default;

    public:
        void paintEvent(QPainter & painter);
        void setDownloadParams(const OSMDownloadAreaDialog::Setup & setup);
        void setVisible(bool value);
        bool isVisible();

        // interface zo StoreConfigInterface
        void storeConfig(QDomDocument & document, QDomElement & rootElement);
        bool restoreConfig(QDomDocument & document);

    public slots:
        void resetDownloadParams();

    private:
        void clearDownloadParams();

    private:
        OSMDownloadAreaDialog::Setup _setup;
        bool _isDownloadParamsSetupEnable;
        bool _isVisible = true;
};

#endif // DOWNLOADAREAHIGHLIGHT_H
