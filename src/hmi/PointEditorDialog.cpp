#include "PointEditorDialog.h"
#include "ui_PointEditorDialog.h"

PointEditorDialog::PointEditorDialog(const QPointF & point, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::PointEditorDialog)
{
    _ui->setupUi(this);

    _ui->positionWidget->setCurrentPosition(point);
}

PointEditorDialog::~PointEditorDialog()
{
    delete _ui;
}

QPointF PointEditorDialog::getCurrentPosition() const
{
    return _ui->positionWidget->getCurrentPosition();
}
