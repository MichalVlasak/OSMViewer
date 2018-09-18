#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "OSMDirectoryPathDialog.h"
#include "Layers/OSMLayer.h"
#include "OSMDownloadAreaDialog.h"
#include "AboutDialog.h"
#include "WgsConversion.h"
#include "GpxFilesListWidget.h"
#include "GpxInfosWidget.h"
#include "GpxFileFinder.h"

#include <QWheelEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QApplication>

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
    _downloader = new OSMTileDownloader(_applicationName, this);
    _downloaderInfoWidget = new OSMTileDownloaderInfoWidget(_downloader, this);
    _downloaderPrepare = new OSMTileDownloaderPrepare(_downloader, _downloaderInfoWidget, this);
    _downloaderSetupWidget = new OSMTileDownloaderSetupWidget(_downloader, this);
    _downloadProjectWidget = new OSMDownloadProjectWidget(this);

    _centerPointsManager = new CenterPointsManager(this);
    _centerPointsWidget = new CenterPointsWidget(_centerPointsManager, this);

    GpxLayer * gpxLayer = _ui->paintWidget->getGpxLayer();

    _gpxManager = new GpxManager();
    gpxLayer->setGpxManager(_gpxManager);
    _gpxFileListWidget = new GpxFilesListWidget(_gpxManager, gpxLayer, this);
    _gpxInfosWidget = new GpxInfosWidget(_gpxManager, gpxLayer, this);

    QObject::connect(_gpxFileListWidget, SIGNAL(changeSelectedGpsSignal(GpxManager::GpxIdVector)), _gpxInfosWidget, SLOT(changeSelectedGps(GpxManager::GpxIdVector)));
    QObject::connect(_gpxFileListWidget, SIGNAL(deleteAllSignal()), _gpxInfosWidget, SLOT(deleteAll()));
    QObject::connect(_gpxFileListWidget, SIGNAL(deleteGpxSignal(int)), _gpxInfosWidget, SLOT(deleteGpx(int)));
    QObject::connect(_gpxInfosWidget, SIGNAL(centerMap(QPoint)), _ui->paintWidget, SLOT(centerMapToPixels(QPoint)));

    _centerPointsDock = new QDockWidget(tr("Center Points"), this);
    _centerPointsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _centerPointsDock->setWidget(_centerPointsWidget);
    _centerPointsDock->hide();
    _centerPointsDock->setFloating(true);
    _centerPointsDock->setObjectName("Center Points");

    _downloaderInfoDock = new QDockWidget(tr("Downloader Info"), this);
    _downloaderInfoDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _downloaderInfoDock->setWidget(_downloaderInfoWidget);
    _downloaderInfoDock->hide();
    _downloaderInfoDock->setFloating(true);
    _downloaderInfoDock->setObjectName("Downloader Info");

    _downloaderSetupDock = new QDockWidget(tr("Downloader Setup"), this);
    _downloaderSetupDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _downloaderSetupDock->setWidget(_downloaderSetupWidget);
    _downloaderSetupDock->hide();
    _downloaderSetupDock->setFloating(true);
    _downloaderSetupDock->setObjectName("Downloader Setup");

    _downloadProjectDock = new QDockWidget(tr("Download Project"), this);
    _downloadProjectDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _downloadProjectDock->setWidget(_downloadProjectWidget);
    _downloadProjectDock->hide();
    _downloadProjectDock->setFloating(true);
    _downloadProjectDock->setObjectName("Download Project");

    _gpxFileListDock = new QDockWidget(tr("GPX List"), this);
    _gpxFileListDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _gpxFileListDock->setWidget(_gpxFileListWidget);
    _gpxFileListDock->hide();
    _gpxFileListDock->setFloating(true);
    _gpxFileListDock->setObjectName("GPX List");

    _gpxInfosDock = new QDockWidget(tr("GPX Info Tables"), this);
    _gpxInfosDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _gpxInfosDock->setWidget(_gpxInfosWidget);
    _gpxInfosDock->hide();
    _gpxInfosDock->setFloating(true);
    _gpxInfosDock->setObjectName("GPX Info Tables");

    addDockWidget(Qt::RightDockWidgetArea, _centerPointsDock);
    addDockWidget(Qt::RightDockWidgetArea, _downloaderInfoDock);
    addDockWidget(Qt::RightDockWidgetArea, _downloaderSetupDock);
    addDockWidget(Qt::RightDockWidgetArea, _downloadProjectDock);
    addDockWidget(Qt::RightDockWidgetArea, _gpxFileListDock);

    QObject::connect(_ui->action_CenterPoints, SIGNAL(triggered(bool)), _centerPointsDock, SLOT(setVisible(bool)));
    QObject::connect(_centerPointsDock, SIGNAL(visibilityChanged(bool)), _ui->action_CenterPoints, SLOT(setChecked(bool)));

    QObject::connect(_ui->action_DownloaderInfo, SIGNAL(triggered(bool)), _downloaderInfoDock, SLOT(setVisible(bool)));
    QObject::connect(_downloaderInfoDock, SIGNAL(visibilityChanged(bool)), _ui->action_DownloaderInfo, SLOT(setChecked(bool)));

    QObject::connect(_ui->action_DownloaderSetup, SIGNAL(triggered(bool)), _downloaderSetupDock, SLOT(setVisible(bool)));
    QObject::connect(_downloaderSetupDock, SIGNAL(visibilityChanged(bool)), _ui->action_DownloaderSetup, SLOT(setChecked(bool)));

    QObject::connect(_ui->action_Projects, SIGNAL(triggered(bool)), _downloadProjectDock, SLOT(setVisible(bool)));
    QObject::connect(_downloadProjectDock, SIGNAL(visibilityChanged(bool)), _ui->action_Projects, SLOT(setChecked(bool)));

    QObject::connect(_ui->action_GPXFilesList, SIGNAL(triggered(bool)), _gpxFileListDock, SLOT(setVisible(bool)));
    QObject::connect(_gpxFileListDock, SIGNAL(visibilityChanged(bool)), _ui->action_GPXFilesList, SLOT(setChecked(bool)));

    QObject::connect(_ui->action_GPXFileInfoTable, SIGNAL(triggered(bool)), _gpxInfosDock, SLOT(setVisible(bool)));
    QObject::connect(_gpxInfosDock, SIGNAL(visibilityChanged(bool)), _ui->action_GPXFileInfoTable, SLOT(setChecked(bool)));

    OSMLayer * osmLayer = _ui->paintWidget->getOSMLayer();

    if(osmLayer != nullptr)
    {
        osmLayer->setOSMTileDownloader(_downloader);

        if(_downloader != nullptr)
        {
            QObject::connect(_downloader, SIGNAL(downloadItemIsDone()), _ui->paintWidget, SLOT(repaint()));
            QObject::connect(_downloader, SIGNAL(downloadingEnable(bool)), _ui->action_EnableDownloading, SLOT(setChecked(bool)));
            QObject::connect(_downloader, SIGNAL(downloadingEnable(bool)), _ui->paintWidget, SLOT(repaint()));

            _appSettings.restoreConfig(_downloader);
        }
    }

    ZoomInfo * zoom = &(_ui->paintWidget->getMapSettings().zoom);

    QObject::connect(zoom, SIGNAL(zoomChanged()), SLOT(zoomChanged()));
    QObject::connect(_ui->paintWidget, SIGNAL(mouseCursorWgsChanged(double,double)), SLOT(mouseCursorWgsChanged(double,double)));
    QObject::connect(_ui->paintWidget, SIGNAL(downloadSelectedArea(AreaGeometry)), SLOT(downloadSelectedArea(AreaGeometry)));
    QObject::connect(_ui->paintWidget, SIGNAL(findInSelectedArea(AreaGeometry)), SLOT(findInSelectedArea(AreaGeometry)));
    QObject::connect(_ui->action_Quit, SIGNAL(triggered(bool)), SLOT(close()));
    QObject::connect(_ui->action_AboutQt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
    QObject::connect(_ui->action_OSM_Directory, SIGNAL(triggered(bool)), SLOT(setOSMDirectoryPath()));
    QObject::connect(_ui->action_EnableDownloading, SIGNAL(triggered(bool)), SLOT(setOSMTileDownloaderEnable(bool)));
    QObject::connect(_ui->action_EnableDownloading, SIGNAL(triggered(bool)), _ui->paintWidget, SLOT(repaint()));
    QObject::connect(_ui->action_AboutOSMViewer, SIGNAL(triggered(bool)), SLOT(showAbout()));

    if(_appSettings.restoreConfig(this) == false)
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

    _appSettings.restoreConfig(&(_ui->paintWidget->getMapSettings()));
    _appSettings.restoreConfig(_ui->paintWidget->getOSMLayer());
    zoomChanged();

    _downloadAreaHighlight = new DownloadAreaHighlight(_ui->paintWidget->getMapSettings(), this);
    _ui->paintWidget->addLayer(_downloadAreaHighlight, "DownloadAreaHighlight");
    _downloaderPrepare->setDownloadAreaHighlight(_downloadAreaHighlight);
    _appSettings.restoreConfig(_downloadAreaHighlight);

    QObject::connect(_downloaderPrepare, SIGNAL(allIsDownloaded()), _downloadAreaHighlight, SLOT(resetDownloadParams()));

    _appSettings.restoreConfig(_centerPointsManager);
    _appSettings.restoreConfig(&_downloadProjectModel);

    _centerPointsWidget->fillPointsList();

    createLanguageMenu();
}

MainWindow::~MainWindow()
{
    _appSettings.storeConfig(&(_ui->paintWidget->getMapSettings()));
    _appSettings.storeConfig(this);
    _appSettings.storeConfig(_ui->paintWidget->getOSMLayer());
    _appSettings.storeConfig(_downloader);
    _appSettings.storeConfig(_downloadAreaHighlight);
    _appSettings.storeConfig(_centerPointsManager);
    _appSettings.storeConfig(&_downloadProjectModel);
    _appSettings.storeConfig(_gpxManager);

    //while(_downloader2->isRunning() == true);

    delete _gpxManager;
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

    if(_downloadProjectWidget != nullptr)
    {
        _downloadProjectWidget->initialize();
    }

    _appSettings.restoreConfig(_gpxManager);
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
    QString latString = WgsConversion::convertDoubleDegToWgs(lat, WgsConversion::Latitude, true, true);
    QString lonString = WgsConversion::convertDoubleDegToWgs(lon, WgsConversion::Longitude, true, true);

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

void MainWindow::showDownloadAreaDialog(OSMDownloadAreaDialog::Setup &setup, const QString & projectName)
{
    try
    {
        OSMDownloadAreaDialog * downloadAreaDialog = new OSMDownloadAreaDialog(setup, projectName, this);

        if(downloadAreaDialog->exec() == QDialog::Accepted)
        {
            setup = downloadAreaDialog->getCurrenSetup();

            _downloaderPrepare->setDownloadParameters(setup, _ui->paintWidget->getOSMLayer()->getOSMDirectorypath());
            _downloaderInfoDock->show();

            _deleteSettings = downloadAreaDialog->getCurrenSetup().deleteSettings;
        }
        else
        {
            if(_downloadAreaHighlight != nullptr)
            {
                _downloadAreaHighlight->resetDownloadParams();
            }
        }
    }
    catch(std::exception & ex)
    {
        QMessageBox::warning(this, tr("Error"), tr("Cannot open download setup dialog: \n") + QString::fromStdString(std::string(ex.what())));
    }
}

void MainWindow::downloadSelectedArea(AreaGeometry geometry)
{
    OSMDownloadAreaDialog::Setup downloadSetup;

    downloadSetup.levelFrom = _ui->paintWidget->getMapSettings().zoom.getCurrentZoomLevel();
    downloadSetup.levelTo = downloadSetup.levelFrom + 1;

    downloadSetup.geometry = geometry;

    downloadSetup.deleteSettings = _deleteSettings;

    showDownloadAreaDialog(downloadSetup);
}

void MainWindow::findInSelectedArea(AreaGeometry geometry)
{
    GpxFileFinder finder(_gpxManager);

    finder.find(geometry);
}

void MainWindow::showAbout()
{
    AboutDialog * about = new AboutDialog(this);

    about->exec();
}

DeleteOldMapsWidget::DeleteSettings MainWindow::getDeleteSettings()
{
    return _deleteSettings;
}

void MainWindow::setDeleteSettings(DeleteOldMapsWidget::DeleteSettings settings)
{
    _deleteSettings = settings;

    OSMLayer * osmLayer = _ui->paintWidget->getOSMLayer();

    if(osmLayer != nullptr)
    {
        osmLayer->setDeleteSettings(settings);
    }
}

CenterPointsManager * MainWindow::getCenterPointsManager()
{
    return _centerPointsManager;
}

CenterPointsWidget * MainWindow::getCenterPointsWidget()
{
    return _centerPointsWidget;
}

void MainWindow::centerToPoint(const CenterPointStruct & point)
{
    _ui->paintWidget->centerToPoint(point);

    zoomChanged();
}

MapSettings & MainWindow::getMapSettings()
{
    return _ui->paintWidget->getMapSettings();
}

OSMDownloadProjectModel & MainWindow::getOSMDownloadProjectModel()
{
    return _downloadProjectModel;
}

void MainWindow::storeConfig(QDomDocument &document, QDomElement &rootElement)
{
    QDomElement mainWindowElement = document.createElement("MainWindowSettings");
    rootElement.appendChild(mainWindowElement);

    QByteArray save = saveState().toHex();
    std::string buffState(save.data(), save.size());
    QDomElement windowStateElement = document.createElement("WindowState");
    mainWindowElement.appendChild(windowStateElement);
    QDomText windowStateText = document.createTextNode(QString::fromStdString(buffState));
    windowStateElement.appendChild(windowStateText);

    save = saveGeometry().toHex();
    std::string buffGeometry(save.data(), save.size());
    QDomElement windowGeometryElement = document.createElement("WindowGeometry");
    mainWindowElement.appendChild(windowGeometryElement);
    QDomText windowGeometryText = document.createTextNode(QString::fromStdString(buffGeometry));
    windowGeometryElement.appendChild(windowGeometryText);

    QDomElement deleteTilesEnabled = document.createElement("DeleteTilesEnabled");
    mainWindowElement.appendChild(deleteTilesEnabled);
    QDomText deleteTilesEnabledText = document.createTextNode(QString::number(getDeleteSettings().deleteEnabled));
    deleteTilesEnabled.appendChild(deleteTilesEnabledText);

    QDomElement deleteTilesType = document.createElement("DeleteTilesType");
    mainWindowElement.appendChild(deleteTilesType);
    QDomText deleteTilesTypeText = document.createTextNode(QString::number(getDeleteSettings().deleteType));
    deleteTilesType.appendChild(deleteTilesTypeText);

    QDomElement deleteTilesTime = document.createElement("DeleteTilesTime");
    mainWindowElement.appendChild(deleteTilesTime);
    QDomText deleteTilesTimeText = document.createTextNode(getDeleteSettings().deleteTime.toString(Qt::DateFormat::ISODate));
    deleteTilesTime.appendChild(deleteTilesTimeText);

    QDomElement currentLanguage = document.createElement("CurrenLanguage");
    mainWindowElement.appendChild(currentLanguage);
    QDomText currentLanguageText = document.createTextNode(_currentLanguages);
    currentLanguage.appendChild(currentLanguageText);
}

bool MainWindow::restoreConfig(QDomDocument &document)
{
    bool result = true;
    QDomElement rootElem = document.firstChildElement("OSMViewer");

    if(rootElem.isNull() == false)
    {
        QDomNodeList mainWindowNodes = rootElem.elementsByTagName("MainWindowSettings");

        for(int iMap = 0; iMap < mainWindowNodes.size(); iMap++)
        {
            QDomNode mapNode = mainWindowNodes.at(iMap);

            if(mapNode.isNull() == false)
            {
                QString value = AppSettings::getValueString(mapNode, "WindowState");

                if(value.isEmpty() == false)
                {
                    QByteArray state = QByteArray(value.toStdString().data(), value.size());

                    state = QByteArray::fromHex(state);

                    if(restoreState(state) == false)
                    {
                        std::cerr << "Cannot restore Main Window state!" << std::endl;
                        result |= false;
                    }
                }

                value = AppSettings::getValueString(mapNode, "WindowGeometry");

                if(value.isEmpty() == false)
                {
                    QByteArray state = QByteArray(value.toStdString().data(), value.size());

                    state = QByteArray::fromHex(state);

                    if(restoreGeometry(state) == false)
                    {
                        std::cerr << "Cannot restore Main Window geometry!" << std::endl;
                        result |= false;
                    }
                }

                DeleteOldMapsWidget::DeleteSettings deleteSettings;
                value = AppSettings::getValueString(mapNode, "DeleteTilesEnabled");

                if(value.isEmpty() == false)
                {
                    bool deleteTiles = (value.toInt() == 0) ? false : true;
                    deleteSettings.deleteEnabled = deleteTiles;
                }
                else
                {
                    deleteSettings.deleteEnabled = false;
                }

                value = AppSettings::getValueString(mapNode, "DeleteTilesType");

                if(value.isEmpty() == false)
                {
                    deleteSettings.deleteType = DeleteOldMapsWidget::DeleteType(value.toInt());
                }
                else
                {
                    deleteSettings.deleteType = DeleteOldMapsWidget::DeleteOldAsTime;
                }

                value = AppSettings::getValueString(mapNode, "DeleteTilesTime");

                if(value.isEmpty() == false)
                {
                    deleteSettings.deleteTime = QDateTime::fromString(value, Qt::DateFormat::ISODate);
                }
                else
                {
                    deleteSettings.deleteTime = QDateTime::currentDateTime();
                }

                setDeleteSettings(deleteSettings);

                value = AppSettings::getValueString(mapNode, "CurrenLanguage");

                if(value.isEmpty() == false)
                {
                    loadLanguage(value);
                }
            }
        }
    }

    return result;
}

GpxManager * MainWindow::getGpxManager()
{
    return _gpxManager;
}

GpxFilesListWidget * MainWindow::getGpxFilesListWidget()
{
    return _gpxFileListWidget;
}

void MainWindow::createLanguageMenu()
{
    QActionGroup* langGroup = new QActionGroup(_ui->menuLanguage);
    langGroup->setExclusive(true);

    QObject::connect(langGroup, SIGNAL(triggered(QAction *)), this, SLOT (changeLanguage(QAction *)));

    QString defaultLocale = QLocale::system().name();
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));

    _langPath = QApplication::applicationDirPath();
    _langPath.append("/languages");
    QDir dir(_langPath);
    QStringList fileNames = dir.entryList(QStringList("OSMViewerTranslation_*.qm"));

    for (int i = 0; i < fileNames.size(); ++i)
    {
        QString locale;

        locale = fileNames[i];
        locale.truncate(locale.lastIndexOf('.'));
        locale.remove(0, locale.indexOf('_') + 1);

        QString lang = QLocale::languageToString(QLocale(locale).language());
        QIcon ico(QString(":/countriesFlags/countriesFlags/%1.png").arg(locale));

        QAction *action = new QAction(ico, lang, this);
        action->setCheckable(true);
        action->setData(locale);

        _ui->menuLanguage->addAction(action);
        langGroup->addAction(action);

        if(_currentLanguages.isEmpty() == false)
        {
            if(locale.compare(_currentLanguages) == 0)
            {
                action->setChecked(true);
                loadLanguage(locale);
            }
        }
        else if(defaultLocale == locale)
        {
            action->setChecked(true);
            loadLanguage(locale);
        }
    }
}

void MainWindow::changeLanguage(QAction* action)
{
    if(action != nullptr)
    {
        loadLanguage(action->data().toString());
    }
}

void switchTranslator(QTranslator& translator, const QString& filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    QString path = QApplication::applicationDirPath();
    path.append("/languages/");
    if(translator.load(path + filename))
    {
        qApp->installTranslator(&translator);
    }
}

void MainWindow::loadLanguage(const QString &language)
{
    if(_currentLanguages != language)
    {
        _currentLanguages = language;
        QLocale locale = QLocale(_currentLanguages);
        QLocale::setDefault(locale);
        QString languageName = QLocale::languageToString(locale.language());
        switchTranslator(_translator, QString("OSMViewerTranslation_%1.qm").arg(language));
        switchTranslator(_translatorQt, QString("qt_%1.qm").arg(language));
    }
}

void MainWindow::changeEvent(QEvent* event)
{
    if(event != nullptr)
    {
        switch(event->type())
        {
            case QEvent::LanguageChange:
                _ui->retranslateUi(this);
                _downloaderInfoWidget->retranslate();
                _downloaderInfoDock->setWindowTitle(QApplication::translate("MainWindow", "Downloader Info", nullptr));
                _downloaderSetupWidget->retranslate();
                _downloaderSetupDock->setWindowTitle(QApplication::translate("MainWindow", "Downloader Setup", nullptr));
                _downloadProjectWidget->retranslate();
                _downloadProjectDock->setWindowTitle(QApplication::translate("MainWindow", "Download Project", nullptr));
                _centerPointsWidget->retranslate();
                _centerPointsDock->setWindowTitle(QApplication::translate("MainWindow", "Center Points", nullptr));
                _gpxFileListWidget->retranslate();
                _gpxFileListDock->setWindowTitle(QApplication::translate("MainWindow", "GPX List", nullptr));
                _gpxInfosWidget->retranslate();
                _gpxInfosDock->setWindowTitle(QApplication::translate("MainWindow", "GPX Info Tables", nullptr));
                break;

            case QEvent::LocaleChange:
                {
                    QString locale = QLocale::system().name();
                    locale.truncate(locale.lastIndexOf('_'));
                    loadLanguage(locale);
                }
                break;

            default:
                break;
        }
    }
    QMainWindow::changeEvent(event);
}

