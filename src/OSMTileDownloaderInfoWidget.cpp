#include "OSMTileDownloaderInfoWidget.h"
#include "ui_OSMTileDownloaderInfoWidget.h"
#include "MainWindow.h"

#include <QPushButton>
#include <QTimer>
#include <QProgressBar>

OSMTileDownloaderInfoWidget::OSMTileDownloaderInfoWidget(OSMTileDownloader * downloader, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::OSMTileDownloaderInfoWidget),
    _downloader(downloader)
{
    _ui->setupUi(this);

    setObjectName("OSMTileDownloaderInfoWidget");
}

OSMTileDownloaderInfoWidget::~OSMTileDownloaderInfoWidget()
{
    delete _ui;
}

void OSMTileDownloaderInfoWidget::initialize()
{

    _timer = new QTimer(this);
    _timer->setInterval(500);
    _timer->start();

    _levelFrom = _levelTo = 0;

    _ui->cancelButton->setDisabled(true);

    _ui->levelValue->setText(tr("0"));
    _ui->columnValue->setText(tr("0"));
    _ui->rowValue->setText(tr("0"));

    DownloadAreaHighlight * highliter = MainWindow::getInstance()->getDownloadAreaHighlight();

    if(highliter != nullptr)
    {
        _ui->highlightDownArea->setChecked(highliter->isVisible());
    }

    //QObject::connect(_downloader, SIGNAL(allItemIsDownloaded()), SLOT(close()));
    QObject::connect(_downloader, SIGNAL(downloadedItem(int,int,int)), SLOT(downloadedItem(int,int,int)));
    QObject::connect(_ui->cancelButton, SIGNAL(clicked(bool)), SLOT(cancel()));
    QObject::connect(_timer, SIGNAL(timeout()), SLOT(updateDownloadInfoLabel()));
    QObject::connect(_ui->highlightDownArea, SIGNAL(toggled(bool)), SLOT(highlightDownArea(bool)));
}

void OSMTileDownloaderInfoWidget::retranslate()
{
    _ui->retranslateUi(this);
}

void OSMTileDownloaderInfoWidget::setLevelInfo(int level, LevelInfo info)
{
    _levelInfo[level] = info;

    LevelInfoMap::iterator it;
    int levelMin = _levelFrom;
    int levelMax = _levelTo;

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
        _levelFrom = levelFrom;
        _levelTo = levelTo;

        _ui->levelProgress->setMinimum(_levelFrom);
        _ui->levelProgress->setMaximum(_levelTo);
    }
}

void OSMTileDownloaderInfoWidget::downloadedItem(int level, int col, int row)
{
    if(row > _ui->rowProgress->value() || col > _ui->columnProgress->value() || level > _ui->levelProgress->value())
    {
        _ui->rowValue->setText(QString::number(row));
        _ui->rowProgress->setValue(row);
    }

    if(col > _ui->columnProgress->value() || level > _ui->levelProgress->value())
    {
        _ui->columnValue->setText(QString::number(col));
        _ui->columnProgress->setValue(col);
    }

    if(level > _ui->levelProgress->value())
    {
        _ui->levelValue->setText(QString::number(level));
        _ui->levelProgress->setValue(level);

        LevelInfoMap::iterator it = _levelInfo.find(level);

        if(it != _levelInfo.end())
        {
            _ui->columnProgress->setMinimum(it->second.colFrom);
            _ui->columnProgress->setMaximum(it->second.colTo);

            _ui->rowProgress->setMinimum(it->second.rowFrom);
            _ui->rowProgress->setMaximum(it->second.rowTo);
        }
    }
}

void OSMTileDownloaderInfoWidget::cancel()
{
    emit cancelDownloading();

    _downloader->cancelDownload();
}

void OSMTileDownloaderInfoWidget::updateDownloadInfoLabel()
{
    bool isDownloading = _downloader->isRunning();
    MainWindow * window = MainWindow::getInstance();

    if(window != nullptr)
    {
        OSMTileDownloaderPrepare * downloadPrepare =  window->getOSMTileDownloaderprepare();

        if(downloadPrepare != nullptr)
        {
            isDownloading |= downloadPrepare->isRunning();
        }
    }

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
    _ui->allDownloadCount->setText(QString::number(_downloader->getAllDownloadCount()));
}

void OSMTileDownloaderInfoWidget::highlightDownArea(bool value)
{
    DownloadAreaHighlight * highliter = MainWindow::getInstance()->getDownloadAreaHighlight();

    if(highliter != nullptr)
    {
        highliter->setVisible(value);
    }
}
