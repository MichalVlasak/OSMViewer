#include "OSMTileDownloaderSetupWidget.h"
#include "ui_OSMTileDownloaderSetupWidget.h"

#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>

OSMTileDownloaderSetupWidget::OSMTileDownloaderSetupWidget(OSMTileDownloader * downloader, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::OSMTileDownloaderSetupWidget),
    _downloader(downloader),
    _isSettingsChanged(false)
{
    _ui->setupUi(this);

    setObjectName("OSMTileDownloaderSetupWidget");

    _ui->threads->setValue(_downloader->getThreads());
    _ui->baseUrlEdit->setText(_downloader->getBaseUrl());
    _ui->baseUrlCombo->insertItems(0, _downloader->getBaseUrlList());
    _ui->settingsChanged->setText(tr(""));

    _timer = new QTimer(this);

    _timer->setInterval(1000);
    _timer->start();

    QObject::connect(_ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(clickedButton(QAbstractButton*)));
    QObject::connect(_ui->baseUrlCombo, SIGNAL(currentIndexChanged(QString)), SLOT(changeBaseUrlFromCombo(QString)));
    QObject::connect(_ui->baseUrlEdit, SIGNAL(textChanged(QString)), SLOT(settingsChanged()));
    QObject::connect(_ui->threads, SIGNAL(valueChanged(int)), SLOT(settingsChanged()));
    QObject::connect(_downloader, SIGNAL(changeThreadsCount(int)), SLOT(downloaderChangeThreadsCount(int)));
    QObject::connect(_downloader, SIGNAL(changeBaseUrl(QString)), SLOT(downloaderChangeBaseUrl(QString)));
    QObject::connect(_timer, SIGNAL(timeout()), SLOT(checkDownloadRunning()));
}

OSMTileDownloaderSetupWidget::~OSMTileDownloaderSetupWidget()
{
    delete _ui;
}

void OSMTileDownloaderSetupWidget::clickedButton(QAbstractButton *button)
{
    QPushButton * btn = qobject_cast<QPushButton *>(button);

    if(btn != nullptr)
    {
        if(btn == _ui->buttonBox->button(QDialogButtonBox::Reset))
        {
            _ui->threads->setValue(_downloader->getThreads());
            _ui->baseUrlEdit->setText(_downloader->getBaseUrl());
        }
        else if(btn == _ui->buttonBox->button(QDialogButtonBox::Ok))
        {
            _downloader->setThreads(size_t(_ui->threads->value()));
            _downloader->setBaseUrl(_ui->baseUrlEdit->text());
            _isSettingsChanged = false;
            updateSettingsChanged();
        }
    }
}

void OSMTileDownloaderSetupWidget::changeBaseUrlFromCombo(QString url)
{
    _ui->baseUrlEdit->setText(url);
}

void OSMTileDownloaderSetupWidget::downloaderChangeBaseUrl(QString url)
{
    QObject::disconnect(_ui->baseUrlEdit, SIGNAL(textChanged(QString)), this, SLOT(settingsChanged()));

    _ui->baseUrlCombo->setCurrentIndex(_ui->baseUrlCombo->findText(url));

    QObject::connect(_ui->baseUrlEdit, SIGNAL(textChanged(QString)), SLOT(settingsChanged()));
}

void OSMTileDownloaderSetupWidget::downloaderChangeThreadsCount(int threads)
{
    QObject::disconnect(_ui->threads, SIGNAL(valueChanged(int)), this, SLOT(settingsChanged()));

    _ui->threads->setValue(threads);

    QObject::connect(_ui->threads, SIGNAL(valueChanged(int)), SLOT(settingsChanged()));
}

void OSMTileDownloaderSetupWidget::settingsChanged()
{
    _isSettingsChanged = true;
    updateSettingsChanged();
}

void OSMTileDownloaderSetupWidget::updateSettingsChanged()
{
    if(_isSettingsChanged == true)
    {
        _ui->settingsChanged->setText(tr("Changed"));
    }
    else
    {
        _ui->settingsChanged->setText(tr(""));
    }
}

void OSMTileDownloaderSetupWidget::checkDownloadRunning()
{
    if(_downloader != nullptr)
    {
        bool disable = _downloader->isRunning();

        _ui->threads->setDisabled(disable);
        _ui->baseUrlEdit->setDisabled(disable);
        _ui->baseUrlCombo->setDisabled(disable);
        _ui->buttonBox->setDisabled(disable);
    }
}
