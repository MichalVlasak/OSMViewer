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

    _ui->threads->setValue(_downloader->getThreads());
    _ui->baseUrlEdit->setText(_downloader->getBaseUrl());
    _ui->baseUrlCombo->insertItems(0, _downloader->getBaseUrlList());
    _ui->settingsChanged->setText(tr(""));

    QObject::connect(_ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(clickedButton(QAbstractButton*)));
    QObject::connect(_ui->baseUrlCombo, SIGNAL(currentIndexChanged(QString)), SLOT(changeBaseUrlFromCombo(QString)));
    QObject::connect(_ui->baseUrlEdit, SIGNAL(textChanged(QString)), SLOT(settingsChanged()));
    QObject::connect(_ui->threads, SIGNAL(valueChanged(int)), SLOT(settingsChanged()));
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
            _downloader->setThreads(_ui->threads->value());
            _isSettingsChanged = false;
            updateSettingsChanged();
        }
    }
}

void OSMTileDownloaderSetupWidget::changeBaseUrlFromCombo(QString url)
{
    _ui->baseUrlEdit->setText(url);
    _downloader->setBaseUrl(url);
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
