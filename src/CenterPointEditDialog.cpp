#include "CenterPointEditDialog.h"
#include "ui_CenterPointEditDialog.h"

#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QGroupBox>

CenterPointEditDialog::CenterPointEditDialog(const CenterPointStruct & point, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::CenterPointEditDialog)
{
    _ui->setupUi(this);

    _ui->name->setText(point.name);
    _ui->latitude->setValue(point.position.y());
    _ui->longitude->setValue(point.position.x());

    if(point.level != CenterPointStruct::UndefinedLevel)
    {
        _ui->levelCheckBox->setChecked(true);
        _ui->level->setValue(point.level);
    }
    else
    {
        _ui->levelCheckBox->setChecked(false);
    }
}

CenterPointEditDialog::~CenterPointEditDialog()
{
    delete _ui;
}
