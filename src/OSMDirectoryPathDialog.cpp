#include "OSMDirectoryPathDialog.h"
#include "ui_OSMDirectoryPathDialog.h"

#include "Layers/OSMLayer.h"

#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

OSMDirectoryPathDialog::OSMDirectoryPathDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OSMDirectoryPathDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->resetDirectoryPath, SIGNAL(clicked(bool)), SLOT(resetToDefault()));
    QObject::connect(ui->selectDirectoryPath, SIGNAL(clicked(bool)), SLOT(selectDirectory()));
}

OSMDirectoryPathDialog::~OSMDirectoryPathDialog()
{
    delete ui;
}

void OSMDirectoryPathDialog::setOSMDirectoryPath(QString path)
{
    ui->osmDirectoryPath->setText(path);
}

QString OSMDirectoryPathDialog::getOSMDirectoryPath()
{
    return ui->osmDirectoryPath->text();
}

void OSMDirectoryPathDialog::resetToDefault()
{
    ui->osmDirectoryPath->setText(OSMLayer::DefaultMapDirectory);
}

void OSMDirectoryPathDialog::selectDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select OSM Directory"), getOSMDirectoryPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(dir.isEmpty() == false)
    {
        ui->osmDirectoryPath->setText(dir);
    }
}
