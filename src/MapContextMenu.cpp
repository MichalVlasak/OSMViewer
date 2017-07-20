#include "MapContextMenu.h"
#include "MainWindow.h"

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

    MainWindow * window = MainWindow::getInstance();

    if(window != nullptr)
    {
        OSMTileDownloader * downloader = window->getOSMTileDownloader();

        if(downloader != nullptr)
        {
            enableDownloading->setChecked(downloader->isDownloadingEnable());

            QObject::connect(enableDownloading, SIGNAL(triggered(bool)), downloader, SLOT(setDownloadingEnable(bool)));
        }
    }

    menu->addAction(enableDownloading);

    QAction * downloadArea = new QAction(tr("Download Viewed Area"), this);
    downloadArea->setDisabled(window->getOSMTileDownloader()->isRunning());
    menu->addAction(downloadArea);
    QObject::connect(downloadArea, SIGNAL(triggered(bool)), SIGNAL(downloadArea()));

    QPoint globalPos = mapToGlobal(QPoint(pos.x(), pos.y()));
    menu->exec(globalPos);
}
