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

    private:
        void updateSettingsChanged();

    private slots:
        void clickedButton(QAbstractButton* button);
        void changeBaseUrlFromCombo(QString url);
        void settingsChanged();
        void checkDownloadRunning();

    private:
        Ui::OSMTileDownloaderSetupWidget *_ui;
        OSMTileDownloader * _downloader;
        bool _isSettingsChanged;
        QTimer * _timer;
};

#endif // DOWNLOADERSETUPWIDGET_H
