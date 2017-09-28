#include "DeleteOldMapsWidget.h"
#include "ui_DeleteOldMapsWidget.h"

#include <QRadioButton>
#include <QDateTimeEdit>
#include <QGroupBox>

DeleteOldMapsWidget::DeleteOldMapsWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::DeleteOldMapsWidget)
{
    _ui->setupUi(this);

    QObject::connect(_ui->deleteOldMapsTime, SIGNAL(toggled(bool)), _ui->time, SLOT(setEnabled(bool)));
}

DeleteOldMapsWidget::~DeleteOldMapsWidget()
{
    delete _ui;
}

DeleteOldMapsWidget::DeleteSettings DeleteOldMapsWidget::getDeleteSettings() const
{
    DeleteSettings settings;

    if(_ui->deleteAllOldMaps->isChecked() == true)
    {
        settings.deleteType = DeleteAll;
    }
    else if(_ui->deleteOldMapsTime->isChecked() == true)
    {
        settings.deleteType = DeleteOldAsTime;
    }

    settings.deleteEnabled = _ui->deleteGroupBox->isChecked();
    settings.deleteTime = _ui->time->dateTime();

    return settings;
}

void DeleteOldMapsWidget::setDeleteSettings(DeleteSettings settings)
{
    if(settings.deleteType == DeleteAll)
    {
        _ui->deleteAllOldMaps->setChecked(true);
    }
    else if(settings.deleteType == DeleteOldAsTime)
    {
        _ui->deleteOldMapsTime->setChecked(true);
    }

    _ui->time->setDateTime(settings.deleteTime);
    _ui->deleteGroupBox->setChecked(settings.deleteEnabled);
}

void DeleteOldMapsWidget::setEnabledDeleteAll(bool enable)
{
    _ui->deleteAllOldMaps->setEnabled(enable);
}
