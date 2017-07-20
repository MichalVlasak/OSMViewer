#include "OSMTileDownloaderInfoWidget.h"
#include "ui_OSMTileDownloaderInfoWidget.h"

#include <QPushButton>
#include <QTimer>

OSMTileDownloaderInfoWidget::OSMTileDownloaderInfoWidget(OSMTileDownloader * downloader, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::OSMTileDownloaderInfoWidget),
    _downloader(downloader)
{
    _ui->setupUi(this);

    _timer = new QTimer(this);
    _timer->setInterval(500);
    _timer->start();

    _ui->cancelButton->setDisabled(true);

    //QObject::connect(_downloader, SIGNAL(allItemIsDownloaded()), SLOT(close()));
    QObject::connect(_downloader, SIGNAL(downloadedItem(int,int,int)), SLOT(downloadedItem(int,int,int)));
    QObject::connect(_ui->cancelButton, SIGNAL(clicked(bool)), SLOT(cancel()));
    QObject::connect(_timer, SIGNAL(timeout()), SLOT(updateDownloadInfoLabel()));
}

OSMTileDownloaderInfoWidget::~OSMTileDownloaderInfoWidget()
{
    delete _ui;
}

void OSMTileDownloaderInfoWidget::setLevelInfo(int level, LevelInfo info)
{
    _levelInfo[level] = info;
}

void OSMTileDownloaderInfoWidget::downloadedItem(int level, int col, int row)
{
    _ui->levelValue->setText(QString::number(level));
    _ui->columnValue->setText(QString::number(col));
    _ui->rowValue->setText(QString::number(row));
}

void OSMTileDownloaderInfoWidget::cancel()
{
    emit cancelDownloading();

    _downloader->cancelDownload();
}

void OSMTileDownloaderInfoWidget::updateDownloadInfoLabel()
{
    bool isDownloading = _downloader->isRunning();
    QString info;

    if(isDownloading == true)
    {
        info = tr("Yes");
        _ui->cancelButton->setEnabled(true);
    }
    else
    {
        info = tr("No");
        _ui->cancelButton->setDisabled(true);
    }

    _ui->downloadInfoLabel->setText(info);
}
