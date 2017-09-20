#include "MapContextMenu.h"
#include "MainWindow.h"
#include "DeleteOldMapsDialog.h"

#include <QMenu>

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

    QAction * downloadArea = new QAction(tr("Download Viewed Area"), this);
    downloadArea->setDisabled(window->getOSMTileDownloader()->isRunning() || window->getOSMTileDownloaderprepare()->isRunning());
    menu->addAction(downloadArea);
    QObject::connect(downloadArea, SIGNAL(triggered(bool)), SIGNAL(downloadArea()));

    QAction * selectAndDownloadArea = new QAction(tr("Select & Download Area"), this);
    selectAndDownloadArea->setDisabled(window->getOSMTileDownloader()->isRunning() || window->getOSMTileDownloaderprepare()->isRunning());
    menu->addAction(selectAndDownloadArea);
    QObject::connect(selectAndDownloadArea, SIGNAL(triggered(bool)), SIGNAL(selectAndDownloadArea()));

    menu->addSeparator();

    QAction * centermap = new QAction(tr("Center Map"), this);
    menu->addAction(centermap);
    QObject::connect(centermap, SIGNAL(triggered(bool)), SLOT(centerMapActivated()));

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
