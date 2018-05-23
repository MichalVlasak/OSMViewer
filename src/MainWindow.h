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
#include "Layers/DownloadAreaHighlight.h"
#include "CenterPointsManager.h"
#include "CenterPointsWidget.h"
#include "OSMDownloadProjectWidget.h"
#include "OSMDownloadProjectModel.h"
#include "StoreConfigInterface.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public StoreConfigInterface
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        static MainWindow * getInstance();

    public:
        OSMTileDownloader * getOSMTileDownloader() { return _downloader; }
        OSMTileDownloaderPrepare * getOSMTileDownloaderprepare() { return _downloaderPrepare; }
        DownloadAreaHighlight * getDownloadAreaHighlight()  { return _downloadAreaHighlight; }
        const QString & getApplicationName() const { return _applicationName; }
        void initialize();

        DeleteOldMapsWidget::DeleteSettings getDeleteSettings();
        void setDeleteSettings(DeleteOldMapsWidget::DeleteSettings settings);
        CenterPointsManager * getCenterPointsManager();
        CenterPointsWidget * getCenterPointsWidget();
        void centerToPoint(const CenterPointStruct & point);
        MapSettings & getMapSettings();
        OSMDownloadProjectModel & getOSMDownloadProjectModel();
        void showDownloadAreaDialog(OSMDownloadAreaDialog::Setup & setup, const QString & projectName = "");

        // interface zo StoreConfigInterface
        void storeConfig(QDomDocument & document, QDomElement & rootElement);
        bool restoreConfig(QDomDocument & document);
        class GpxManager * getGpxManager();

    private:
        void keyPressEvent(QKeyEvent * event);

    private slots:
        void zoomChanged();
        void mouseCursorWgsChanged(double lat, double lon);
        void setOSMDirectoryPath();
        void setOSMTileDownloaderEnable(bool enable);
        void downloadArea();
        void downloadSelectedArea(QPointF topLeft, QPointF bottomRight);
        void showAbout();

    private:
        Ui::MainWindow * _ui;
        AppSettings _appSettings;
        OSMTileDownloader * _downloader = nullptr;
        QDockWidget * _downloaderInfoDock = nullptr;
        QDockWidget * _downloaderSetupDock = nullptr;
        QDockWidget * _downloadProjectDock = nullptr;
        OSMTileDownloaderInfoWidget * _downloaderInfoWidget = nullptr;
        OSMTileDownloaderPrepare * _downloaderPrepare = nullptr;
        OSMTileDownloaderSetupWidget * _downloaderSetupWidget = nullptr;
        OSMDownloadProjectWidget * _downloadProjectWidget = nullptr;
        QString _applicationName;
        DownloadAreaHighlight * _downloadAreaHighlight = nullptr;

        QLabel * _zoomLevelLabel;
        QLabel * _zoomScaleLabel;
        QLabel * _mouseWgs;
        QRect _rect;
        double _lastMouseLat = 0.;
        double _lastMouseLon = 0.;

        DeleteOldMapsWidget::DeleteSettings _deleteSettings;
        CenterPointsManager * _centerPointsManager = nullptr;
        CenterPointsWidget * _centerPointsWidget = nullptr;
        QDockWidget * _centerPointsDock = nullptr;
        OSMDownloadProjectModel _downloadProjectModel;
        class GpxWidget * _gpxWidget = nullptr;
        QDockWidget * _gpxDock = nullptr;
        class GpxManager * _gpxManager = nullptr;
};

#endif // MAINWINDOW_H
