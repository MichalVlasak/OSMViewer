#ifndef DOWNLOADAREAHIGHLIGHT_H
#define DOWNLOADAREAHIGHLIGHT_H

#include "BaseLayer.h"
#include "../OSMDownloadAreaDialog.h"

#include <QObject>

class DownloadAreaHighlight : public QObject, public BaseLayer
{
        Q_OBJECT

    public:
        explicit DownloadAreaHighlight(MapSettings & mapSettings, QObject * parent = nullptr);

    public:
        void paintEvent(QPaintEvent * paintEvent);
        void setDownloadParams(OSMDownloadAreaDialog::Setup setup);
        void setVisible(bool value);
        bool isVisible();

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
