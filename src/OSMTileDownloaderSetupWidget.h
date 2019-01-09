#ifndef DOWNLOADERSETUPWIDGET_H
#define DOWNLOADERSETUPWIDGET_H

#include <QWidget>
#include <QDialogButtonBox>

#include "OSMTileDownloader.h"

namespace Ui {
class OSMTileDownloaderSetupWidget;
}

class OSMTileDownloaderSetupWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit OSMTileDownloaderSetupWidget(OSMTileDownloader * downloader, QWidget *parent = 0);
        ~OSMTileDownloaderSetupWidget();

        void retranslate();

    private:
        void updateSettingsChanged();

    private slots:
        void clickedButton(QAbstractButton* button);
        void changeBaseUrlFromCombo(QString url);
        void settingsChanged();
        void checkDownloadRunning();
        void downloaderChangeBaseUrl(QString url);
        void downloaderChangeThreadsCount(int threads);

    private:
        Ui::OSMTileDownloaderSetupWidget *_ui = nullptr;
        OSMTileDownloader * _downloader = nullptr;
        bool _isSettingsChanged;
        QTimer * _timer = nullptr;
};

#endif // DOWNLOADERSETUPWIDGET_H
