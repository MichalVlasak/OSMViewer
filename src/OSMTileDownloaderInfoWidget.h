#ifndef OSMTILEDOWNLOADERINFOFORM_H
#define OSMTILEDOWNLOADERINFOFORM_H

#include <QWidget>

#include <map>

#include "OSMTileDownloader.h"

namespace Ui {
class OSMTileDownloaderInfoWidget;
}

class OSMTileDownloaderInfoWidget : public QWidget
{
        Q_OBJECT

    public:
        struct LevelInfo
        {
                int colFrom;
                int colTo;
                int rowFrom;
                int rowTo;
        };

    public:
        explicit OSMTileDownloaderInfoWidget(OSMTileDownloader * downloader, QWidget *parent = 0);
        ~OSMTileDownloaderInfoWidget();

    public:
        void setLevelInfo(int level, LevelInfo info);
        void setLevelRange(int levelFrom, int levelTo);

    signals:
        void cancelDownloading();

    private slots:
        void downloadedItem(int level, int col, int row);
        void cancel();
        void updateDownloadInfoLabel();

    private:
        typedef std::map<int, LevelInfo> LevelInfoMap;

    private:
        Ui::OSMTileDownloaderInfoWidget *_ui;
        LevelInfoMap _levelInfo;
        OSMTileDownloader * _downloader;
        QTimer * _timer;
};

#endif // OSMTILEDOWNLOADERINFOFORM_H
