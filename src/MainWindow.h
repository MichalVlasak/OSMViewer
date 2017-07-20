#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDockWidget>

#include "AppSettings.h"
#include "OSMTileDownloader.h"
#include "OSMTileDownloaderInfoWidget.h"
#include "OSMTileDownloaderPrepare.h"
#include "OSMTileDownloaderSetupWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        static MainWindow * getInstance();

    public:
        OSMTileDownloader * getOSMTileDownloader() { return _downloader; }

    private:
        void keyPressEvent(QKeyEvent * event);

    private slots:
        void zoomChanged();
        void mouseCursorWgsChanged(double lat, double lon);
        void setOSMDirectoryPath();
        void setOSMTileDownloaderEnable(bool enable);
        void downloadArea();
        void downloadSelectedArea(QPointF topLeft, QPointF bottomRight);

    private:
        Ui::MainWindow * _ui;
        AppSettings _appSettings;
        OSMTileDownloader * _downloader = nullptr;
        QDockWidget * _downloaderInfoDock = nullptr;
        QDockWidget * _downloaderSetupDock = nullptr;
        OSMTileDownloaderInfoWidget * _downloaderInfoWidget = nullptr;
        OSMTileDownloaderPrepare * _downloaderPrepare = nullptr;
        OSMTileDownloaderSetupWidget * _downloaderSetupWidget = nullptr;

        QLabel * _zoomLevelLabel;
        QLabel * _zoomScaleLabel;
        QLabel * _mouseWgs;
        QRect _rect;
};

#endif // MAINWINDOW_H
