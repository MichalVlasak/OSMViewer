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
        void setVisible(bool value);
        bool isVisible();

    private:
        void clearDownloadParams();

    private:
        OSMDownloadAreaDialog::Setup _setup;
        bool _isDownloadParamsSetupEnable;
        bool _isVisible = true;
};

#endif // DOWNLOADAREAHIGHLIGHT_H
