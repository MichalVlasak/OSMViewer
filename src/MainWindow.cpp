#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "OSMDirectoryPathDialog.h"
#include "Layers/GridLayer.h"
#include "Layers/OSMLayer.h"
#include "OSMDownloadAreaDialog.h"
#include "AboutDialog.h"

#include <QWheelEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _applicationName = tr("Open Street Map Viewer");

    this->setWindowTitle(_applicationName);

    _zoomLevelLabel = new QLabel("Zoom Level: ");
    _zoomScaleLabel = new QLabel("Zoom Scale: ");
    _mouseWgs = new QLabel("Mouse WGS Pos: ");

    _ui->statusBar->addWidget(_zoomLevelLabel);
    _ui->statusBar->addWidget(_zoomScaleLabel);
    _ui->statusBar->addWidget(_mouseWgs);

    //_downloader = new OSMTileDownloader(this);
    _downloader = new OSMTileDownloader(this);
    _downloaderInfoWidget = new OSMTileDownloaderInfoWidget(_downloader, this);
    _downloaderPrepare = new OSMTileDownloaderPrepare(_downloader, _downloaderInfoWidget, this);
    _downloaderSetupWidget = new OSMTileDownloaderSetupWidget(_downloader, this);

    _downloaderInfoDock = new QDockWidget(tr("Downloader Info"), this);
    _downloaderInfoDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _downloaderInfoDock->setWidget(_downloaderInfoWidget);
    _downloaderInfoDock->hide();
    _downloaderInfoDock->setFloating(true);

    _downloaderSetupDock = new QDockWidget(tr("Downloader Setup"), this);
    _downloaderSetupDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _downloaderSetupDock->setWidget(_downloaderSetupWidget);
    _downloaderSetupDock->hide();
    _downloaderSetupDock->setFloating(true);

    addDockWidget(Qt::RightDockWidgetArea, _downloaderInfoDock);
    addDockWidget(Qt::RightDockWidgetArea, _downloaderSetupDock);

    QObject::connect(_ui->action_DownloaderInfo, SIGNAL(triggered(bool)), _downloaderInfoDock, SLOT(setVisible(bool)));
    QObject::connect(_downloaderInfoDock, SIGNAL(visibilityChanged(bool)), _ui->action_DownloaderInfo, SLOT(setChecked(bool)));

    QObject::connect(_ui->action_DownloaderSetup, SIGNAL(triggered(bool)), _downloaderSetupDock, SLOT(setVisible(bool)));
    QObject::connect(_downloaderSetupDock, SIGNAL(visibilityChanged(bool)), _ui->action_DownloaderSetup, SLOT(setChecked(bool)));

    OSMLayer * osmLayer = _ui->paintWidget->getOSMLayer();

    if(osmLayer != nullptr)
    {
        osmLayer->setOSMTileDownloader(_downloader);

        if(_downloader != nullptr)
        {
            QObject::connect(_downloader, SIGNAL(downloadItemIsDone()), _ui->paintWidget, SLOT(repaint()));
            QObject::connect(_downloader, SIGNAL(downloadingEnable(bool)), _ui->action_EnableDownloading, SLOT(setChecked(bool)));
            QObject::connect(_downloader, SIGNAL(downloadingEnable(bool)), _ui->paintWidget, SLOT(repaint()));

            _appSettings.restoreDownloadSettings(_downloader);
        }
    }

    ZoomInfo * zoom = &(_ui->paintWidget->getMapSettings().zoom);

    QObject::connect(zoom, SIGNAL(zoomChanged()), SLOT(zoomChanged()));
    QObject::connect(_ui->paintWidget, SIGNAL(mouseCursorWgsChanged(double,double)), SLOT(mouseCursorWgsChanged(double,double)));
    QObject::connect(_ui->paintWidget, SIGNAL(downloadArea()), SLOT(downloadArea()));
    QObject::connect(_ui->paintWidget, SIGNAL(downloadSelectedArea(QPointF,QPointF)), SLOT(downloadSelectedArea(QPointF,QPointF)));
    QObject::connect(_ui->action_Quit, SIGNAL(triggered(bool)), SLOT(close()));
    QObject::connect(_ui->action_AboutQt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
    QObject::connect(_ui->action_OSM_Directory, SIGNAL(triggered(bool)), SLOT(setOSMDirectoryPath()));
    QObject::connect(_ui->action_EnableDownloading, SIGNAL(triggered(bool)), SLOT(setOSMTileDownloaderEnable(bool)));
    QObject::connect(_ui->action_EnableDownloading, SIGNAL(triggered(bool)), _ui->paintWidget, SLOT(repaint()));
    QObject::connect(_ui->action_AboutOSMViewer, SIGNAL(triggered(bool)), SLOT(showAbout()));

    if(_appSettings.restoreMainWindowSettings(this) == false)
    {
        QDesktopWidget desktop;

        int width = 1000;
        int height = 800;

        int screenWidth = desktop.screen()->width();
        int screenHeight = desktop.screen()->height();

        setGeometry((screenWidth / 2) - (width / 2), (screenHeight / 2) - (height / 2), width, height);
    }

    int heightOffset = _ui->menuBar->height() + _ui->statusBar->height();

    QPointF center(width() / 2, (height() - heightOffset) / 2);

    _ui->paintWidget->setCenterPosition(center);

    _appSettings.restoreMapSettings(_ui->paintWidget->getMapSettings());
    _appSettings.restoreOSMDirectoryPath(_ui->paintWidget->getOSMLayer());
    zoomChanged();

    _downloadAreaHighlight = new DownloadAreaHighlight(_ui->paintWidget->getMapSettings(), this);
    _ui->paintWidget->addLayer(_downloadAreaHighlight, "DownloadAreaHighlight");
    _downloaderPrepare->setDownloadAreaHighlight(_downloadAreaHighlight);
    _appSettings.restoreDownloadAreaHighlightSettings(_downloadAreaHighlight);
    QObject::connect(_downloader, SIGNAL(allItemIsDownloaded()), _downloadAreaHighlight, SLOT(resetDownloadParams()));
}

MainWindow::~MainWindow()
{
    _appSettings.storeMapSettings(_ui->paintWidget->getMapSettings());
    _appSettings.storeMainWindowSettings(this);
    _appSettings.storeOSMDirectoryPath(_ui->paintWidget->getOSMLayer());
    _appSettings.storeDownloadSettings(_downloader);
    _appSettings.storeDownloadAreaHighlightSettings(_downloadAreaHighlight);

    //while(_downloader2->isRunning() == true);

    delete _downloader;
    delete _ui;
}

MainWindow * mainWindow = nullptr;

MainWindow * MainWindow::getInstance()
{
    if(mainWindow == nullptr)
    {
        mainWindow = new MainWindow();
    }

    return mainWindow;
}

void MainWindow::initialize()
{
    if(_downloaderInfoWidget != nullptr)
    {
        _downloaderInfoWidget->initialize();
    }
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    QMainWindow::keyPressEvent(event);

    _ui->paintWidget->keyPressEvent(event);
}

void MainWindow::zoomChanged()
{
    QString zoomLevel = tr("Zoom Level: ") + QString::number(_ui->paintWidget->getMapSettings().zoom.getCurrentZoomLevel());
    QString zoomScale = tr("Zoom Scale: ") + _ui->paintWidget->getMapSettings().zoom.getCurrentScaleString();

    _zoomLevelLabel->setText(zoomLevel);
    _zoomScaleLabel->setText(zoomScale);

    mouseCursorWgsChanged(_lastMouseLat, _lastMouseLon);
}

void MainWindow::mouseCursorWgsChanged(double lat, double lon)
{
    QString latString = GridLayer::getPrintableDegree(lat * 60. * 60., GridLayer::Latitude, true, true);
    QString lonString = GridLayer::getPrintableDegree(lon * 60. * 60., GridLayer::Longitude, true, true);

    int level = _ui->paintWidget->getMapSettings().zoom.getCurrentZoomLevel();
    int column = _ui->paintWidget->getMapSettings().long2tilex(lon, level);
    int row = _ui->paintWidget->getMapSettings().lat2tiley(lat, level);
    QString lcr = QString("L:%1/C:%2/R:%3").arg(level).arg(column).arg(row); // level, column, row

    _mouseWgs->setText(lonString + " " + latString + " (" + lcr + ")");

    _lastMouseLat = lat;
    _lastMouseLon = lon;
}

void MainWindow::setOSMDirectoryPath()
{
    OSMDirectoryPathDialog * dialog = new OSMDirectoryPathDialog(this);

    dialog->setOSMDirectoryPath(_ui->paintWidget->getOSMDirectoryPath());

    if(dialog->exec() == QDialog::Accepted)
    {
        _ui->paintWidget->setOSMDirectoryPath(dialog->getOSMDirectoryPath());
    }
}

void MainWindow::setOSMTileDownloaderEnable(bool enable)
{
    if(_downloader != nullptr)
    {
        _downloader->setDownloadingEnable(enable);
    }
}

void MainWindow::downloadArea()
{
    OSMDownloadAreaDialog::Setup downloadSetup;

    downloadSetup.levelFrom = _ui->paintWidget->getMapSettings().zoom.getCurrentZoomLevel();
    downloadSetup.levelTo = downloadSetup.levelFrom + 1;

    downloadSetup.latFrom = _ui->paintWidget->getTopLeft().y();
    downloadSetup.lonFrom = _ui->paintWidget->getTopLeft().x();

    downloadSetup.latTo = _ui->paintWidget->getBottomRight().y();
    downloadSetup.lonTo = _ui->paintWidget->getBottomRight().x();

    OSMDownloadAreaDialog * downloadAreaDialog = new OSMDownloadAreaDialog(downloadSetup, this);

    if(downloadAreaDialog->exec() == QDialog::Accepted)
    {
        downloadSetup = downloadAreaDialog->getCurrenSetup();

        _downloaderPrepare->setDownloadParameters(downloadSetup, _ui->paintWidget->getOSMLayer()->getOSMDirectorypath());
        _downloaderInfoDock->show();
    }
}

void MainWindow::downloadSelectedArea(QPointF topLeft, QPointF bottomRight)
{
    OSMDownloadAreaDialog::Setup downloadSetup;

    downloadSetup.levelFrom = _ui->paintWidget->getMapSettings().zoom.getCurrentZoomLevel();
    downloadSetup.levelTo = downloadSetup.levelFrom + 1;

    downloadSetup.latFrom = topLeft.y();
    downloadSetup.lonFrom = topLeft.x();

    downloadSetup.latTo = bottomRight.y();
    downloadSetup.lonTo = bottomRight.x();

    if(downloadSetup.latFrom < downloadSetup.latTo)
    {
        std::swap(downloadSetup.latFrom, downloadSetup.latTo);
    }

    if(downloadSetup.lonFrom > downloadSetup.lonTo)
    {
        std::swap(downloadSetup.lonFrom, downloadSetup.lonTo);
    }

    OSMDownloadAreaDialog * downloadAreaDialog = new OSMDownloadAreaDialog(downloadSetup, this);

    if(downloadAreaDialog->exec() == QDialog::Accepted)
    {
        downloadSetup = downloadAreaDialog->getCurrenSetup();

        _downloaderPrepare->setDownloadParameters(downloadSetup, _ui->paintWidget->getOSMLayer()->getOSMDirectorypath());
        _downloaderInfoDock->show();
    }
    else
    {
        if(_downloadAreaHighlight != nullptr)
        {
            _downloadAreaHighlight->resetDownloadParams();
        }
    }
}

void MainWindow::showAbout()
{
    AboutDialog * about = new AboutDialog(this);

    about->exec();
}
