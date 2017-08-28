#include "OSMTileDownloaderInfoWidget.h"
#include "ui_OSMTileDownloaderInfoWidget.h"

#include <QPushButton>
#include <QTimer>
#include <QProgressBar>

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

    _ui->levelValue->setText(tr("0"));
    _ui->columnValue->setText(tr("0"));
    _ui->rowValue->setText(tr("0"));

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

    LevelInfoMap::iterator it;
    int levelMin = _ui->levelProgress->minimum();
    int levelMax = _ui->levelProgress->maximum();

    for(it = _levelInfo.begin(); it != _levelInfo.end(); ++it)
    {
        int level = it->first;

        if(level < levelMin)
        {
            levelMin = level;
        }

        if(level > levelMax)
        {
            levelMax = level;
        }
    }

    if(levelMin <= levelMax)
    {
        _ui->levelProgress->setMaximum(levelMax);
        _ui->levelProgress->setMinimum(levelMin);
    }
}

void OSMTileDownloaderInfoWidget::setLevelRange(int levelFrom, int levelTo)
{
    if(levelFrom <= levelTo)
    {
        _ui->levelProgress->setMaximum(levelTo);
        _ui->levelProgress->setMinimum(levelFrom);
    }
}

void OSMTileDownloaderInfoWidget::downloadedItem(int level, int col, int row)
{
    _ui->levelValue->setText(QString::number(level));
    _ui->columnValue->setText(QString::number(col));
    _ui->rowValue->setText(QString::number(row));

    LevelInfoMap::iterator it = _levelInfo.find(level);

    if(it != _levelInfo.end())
    {
        _ui->columnProgress->setMinimum(it->second.colFrom);
        _ui->columnProgress->setMaximum(it->second.colTo);

        _ui->rowProgress->setMinimum(it->second.rowFrom);
        _ui->rowProgress->setMaximum(it->second.rowTo);
    }

    _ui->levelProgress->setValue(level);
    _ui->columnProgress->setValue(col);
    _ui->rowProgress->setValue(row);
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

        _ui->levelProgress->setMinimum(0);
        _ui->levelProgress->setMaximum(1);

        _ui->columnProgress->setMinimum(0);
        _ui->columnProgress->setMaximum(1);

        _ui->rowProgress->setMinimum(0);
        _ui->rowProgress->setMaximum(1);

        _ui->levelProgress->setValue(0);
        _ui->columnProgress->setValue(0);
        _ui->rowProgress->setValue(0);

        _ui->levelValue->setText(tr("0"));
        _ui->columnValue->setText(tr("0"));
        _ui->rowValue->setText(tr("0"));
    }

    _ui->downloadInfoLabel->setText(info);
}
