#include "GpxWidget.h"
#include "ui_GpxWidget.h"

#include <QFileDialog>

GpxWidget::GpxWidget(GpxManager * gpxManager, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GpxWidget),
    _gpxManager(gpxManager)
{
    _ui->setupUi(this);

    QObject::connect(_ui->addFile, SIGNAL(clicked(bool)), SLOT(addFile()));
}

GpxWidget::~GpxWidget()
{
    delete _ui;
}

void GpxWidget::addFile()
{
    if(_gpxManager != nullptr)
    {
        QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select GPX Files"), _gpxManager->getLastPathToGpxFiles(), tr("GPX (*.gpx)"));

        _gpxManager->loadGpxFiles(fileNames);
    }
}
