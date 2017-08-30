#ifndef DOWNLOADAREAHIGHLIGHT_H
#define DOWNLOADAREAHIGHLIGHT_H

#include "BaseLayer.h"
#include "../OSMDownloadAreaDialog.h"

class DownloadAreaHighlight : public BaseLayer
{
    public:
        explicit DownloadAreaHighlight(MapSettings & mapSettings);

    public:
        void paintEvent(QPaintEvent * paintEvent);
        void setDownloadParams(OSMDownloadAreaDialog::Setup setup);
        void resetDownloadParams();

    private:
        void clearDownloadParams();

    private:
        OSMDownloadAreaDialog::Setup _setup;
        bool _isDownloadParamsSetupEnable;
};

#endif // DOWNLOADAREAHIGHLIGHT_H
