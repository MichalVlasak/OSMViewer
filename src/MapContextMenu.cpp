#include "MapContextMenu.h"
#include "MainWindow.h"
#include "DeleteOldMapsDialog.h"
#include "CenterPointEditDialog.h"
#include "WgsConversion.h"
#include "GpxManager.h"

#include <QMenu>
#include <QClipboard>
#include <QApplication>

MapContextMenu::MapContextMenu(QWidget * parent)
    : QWidget(parent)
{
}

void MapContextMenu::show(const QPointF &pos)
{
    QMenu * menu = new QMenu(this);

    QAction * menuTitle = new QAction(tr("Map Menu"), this);

    QFont font = this->font();
    font.setBold(true);

    menuTitle->setEnabled(false);
    menuTitle->setFont(font);

    menu->addAction(menuTitle);
    menu->addSeparator();

    QAction * enableDownloading = new QAction(tr("Enable Downloading"), this);

    enableDownloading->setCheckable(true);
    QObject::connect(enableDownloading, SIGNAL(triggered(bool)), SLOT(setEnableDownloading(bool)));

    MainWindow * window = MainWindow::getInstance();

    if(window != nullptr)
    {
        OSMTileDownloader * downloader = window->getOSMTileDownloader();

        if(downloader != nullptr)
        {
            enableDownloading->setChecked(downloader->isDownloadingEnable());
        }
    }

    menu->addAction(enableDownloading);

    if(window != nullptr)
    {
        QAction * downloadArea = new QAction(tr("Download Viewed Area"), this);
        downloadArea->setDisabled(window->getOSMTileDownloader()->isRunning() || window->getOSMTileDownloaderprepare()->isRunning());
        menu->addAction(downloadArea);
        QObject::connect(downloadArea, SIGNAL(triggered(bool)), SIGNAL(downloadArea()));

        QMenu * downloadSubMenu = new QMenu(tr("Select & Download Area"), menu);
        menu->addMenu(downloadSubMenu);

        QAction * selectAndDownloadAreaRec = new QAction(tr("Define as Rectangle"), this);
        selectAndDownloadAreaRec->setDisabled(window->getOSMTileDownloader()->isRunning() || window->getOSMTileDownloaderprepare()->isRunning());
        downloadSubMenu->addAction(selectAndDownloadAreaRec);
        QObject::connect(selectAndDownloadAreaRec, SIGNAL(triggered(bool)), SIGNAL(selectAndDownloadAreaRec()));

        QAction * selectAndDownloadAreaPoly = new QAction(tr("Define as Polygon"), this);
        selectAndDownloadAreaPoly->setDisabled(window->getOSMTileDownloader()->isRunning() || window->getOSMTileDownloaderprepare()->isRunning());
        downloadSubMenu->addAction(selectAndDownloadAreaPoly);
        QObject::connect(selectAndDownloadAreaPoly, SIGNAL(triggered(bool)), SIGNAL(selectAndDownloadAreaPoly()));

        QAction * selectAndDownloadAreaLine = new QAction(tr("Define as Line"), this);
        selectAndDownloadAreaLine->setDisabled(window->getOSMTileDownloader()->isRunning() || window->getOSMTileDownloaderprepare()->isRunning());
        downloadSubMenu->addAction(selectAndDownloadAreaLine);
        QObject::connect(selectAndDownloadAreaLine, SIGNAL(triggered(bool)), SIGNAL(selectAndDownloadAreaLine()));

        QAction * selectAndDownloadAreaCircle = new QAction(tr("Define as Circle"), this);
        selectAndDownloadAreaCircle->setDisabled(window->getOSMTileDownloader()->isRunning() || window->getOSMTileDownloaderprepare()->isRunning());
        downloadSubMenu->addAction(selectAndDownloadAreaCircle);
        QObject::connect(selectAndDownloadAreaCircle, SIGNAL(triggered(bool)), SIGNAL(selectAndDownloadAreaCircle()));

        QMenu * findSubMenu = new QMenu(tr("Find GPX's"), menu);
        menu->addMenu(findSubMenu);

        QAction * selectAndFindAreaRec = new QAction(tr("Define as Rectangle"), this);
        selectAndFindAreaRec->setDisabled(window->getOSMTileDownloader()->isRunning() || window->getOSMTileDownloaderprepare()->isRunning());
        findSubMenu->addAction(selectAndFindAreaRec);
        QObject::connect(selectAndFindAreaRec, SIGNAL(triggered(bool)), SIGNAL(selectAndFindAreaRec()));

        QAction * selectAndFindAreaPoly = new QAction(tr("Define as Polygon"), this);
        selectAndFindAreaPoly->setDisabled(window->getOSMTileDownloader()->isRunning() || window->getOSMTileDownloaderprepare()->isRunning());
        findSubMenu->addAction(selectAndFindAreaPoly);
        QObject::connect(selectAndFindAreaPoly, SIGNAL(triggered(bool)), SIGNAL(selectAndFindAreaPoly()));

        MainWindow * mainWin = MainWindow::getInstance();

        if(mainWin != nullptr)
        {
            GpxManagerPtr gpxManager = mainWin->getGpxManager();

            if(gpxManager != nullptr)
            {
                bool isGpxsLoaded = gpxManager->isAllLoaaded();

                selectAndFindAreaRec->setEnabled(isGpxsLoaded);
                selectAndFindAreaPoly->setEnabled(isGpxsLoaded);
            }
        }
    }

    menu->addSeparator();

    QAction * centerMap = new QAction(tr("Center Map"), this);
    menu->addAction(centerMap);
    QObject::connect(centerMap, SIGNAL(triggered(bool)), SLOT(centerMapActivated()));

    QAction * centerPoints = new QAction(tr("Add to Center Points"), this);
    menu->addAction(centerPoints);
    QObject::connect(centerPoints, SIGNAL(triggered(bool)), SLOT(addToCenterPoint()));

    QAction * homePoint = new QAction(tr("Set as Home Point"), this);
    menu->addAction(homePoint);
    QObject::connect(homePoint, SIGNAL(triggered(bool)), SLOT(setAsHomePoint()));

    menu->addSeparator();

    QAction * pointToClipboard = new QAction(tr("Save position to Clipboard"), this);
    menu->addAction(pointToClipboard);
    QObject::connect(pointToClipboard, SIGNAL(triggered(bool)), SLOT(pointToClipboard()));

    _pos = QPoint(pos.x(), pos.y());

    QPoint globalPos = mapToGlobal(_pos);
    menu->exec(globalPos);
}

void MapContextMenu::centerMapActivated()
{
    emit centerMap(_pos);
}

void MapContextMenu::setEnableDownloading(bool enabled)
{
    MainWindow * window = MainWindow::getInstance();

    if(window != nullptr)
    {
        OSMTileDownloader * downloader = window->getOSMTileDownloader();

        if(downloader != nullptr)
        {
            if(enabled == true)
            {
                DeleteOldMapsWidget::DeleteSettings settings = window->getDeleteSettings();

                if(getDeleteMapsSettings(settings) == true)
                {
                    window->setDeleteSettings(settings);
                }
            }

            downloader->setDownloadingEnable(enabled);
        }
    }
}

bool MapContextMenu::getDeleteMapsSettings(DeleteOldMapsWidget::DeleteSettings & settings)
{
    DeleteOldMapsDialog * dialog = new DeleteOldMapsDialog(this);
    dialog->setDeleteSettings(settings);
    dialog->setEnabledDeleteAll(false);

    if(dialog->exec() == QDialog::Accepted)
    {
        settings = dialog->getDeleteSettings();

        return true;
    }

    return false;
}

void MapContextMenu::addToCenterPoint()
{
    MainWindow * window = MainWindow::getInstance();

    if(window != nullptr)
    {
        MapSettings & mapSettings = window->getMapSettings();

        int level = mapSettings.zoom.getCurrentZoomLevel();
        double lat = mapSettings.getLatForPixel(mapSettings.windowPixelToMapPixelY(_pos.y()));
        double lon = mapSettings.getLonForPixelOld((_pos.x()));

        CenterPointStruct point;

        point.level = level;
        point.position = QPointF(lon, lat);

        CenterPointEditDialog * dialog = new CenterPointEditDialog(point, this);

        if(dialog->exec() == QDialog::Accepted)
        {
            point = dialog->getCenterPoint();

            CenterPointsManager * pointsManager = window->getCenterPointsManager();
            {
                CenterPointsWidget * pointsWidget = window->getCenterPointsWidget();

                if(pointsWidget != nullptr)
                {
                    QString groupName = pointsWidget->getCurrentGroupName();

                    pointsWidget->setLastAdded(point.name);
                    pointsManager->addCenterPoint(groupName, point, true);
                }
            }
        }
    }
}

void MapContextMenu::setAsHomePoint()
{
    MainWindow * window = MainWindow::getInstance();

    if(window != nullptr)
    {
        MapSettings & mapSettings = window->getMapSettings();

        int level = mapSettings.zoom.getCurrentZoomLevel();
        double lat = mapSettings.getLatForPixel(mapSettings.windowPixelToMapPixelY(_pos.y()));
        double lon = mapSettings.getLonForPixelOld((_pos.x()));

        CenterPointStruct point;

        point.level = level;
        point.position = QPointF(lon, lat);

        CenterPointEditDialog * dialog = new CenterPointEditDialog(point, this);

        if(dialog->exec() == QDialog::Accepted)
        {
            point = dialog->getCenterPoint();

            CenterPointsManager * pointsManager = window->getCenterPointsManager();
            {
                pointsManager->setHomeCenterPoint(point);
            }
        }
    }
}

void MapContextMenu::pointToClipboard()
{
    MainWindow * window = MainWindow::getInstance();

    if(window != nullptr)
    {
        MapSettings & mapSettings = window->getMapSettings();

        double lat = mapSettings.getLatForPixel(mapSettings.windowPixelToMapPixelY(_pos.y()));
        double lon = mapSettings.getLonForPixelOld((_pos.x()));

        QString latString = WgsConversion::convertDoubleDegToWgs(lat, WgsConversion::Latitude, true, true);
        QString lonString = WgsConversion::convertDoubleDegToWgs(lon, WgsConversion::Longitude, true, true);

        QClipboard * clipboard = QApplication::clipboard();

        if(clipboard != nullptr)
        {
            clipboard->setText(latString + " " + lonString);
        }
    }
}
