#include "PositionEditorWidget.h"
#include "ui_PositionEditorWidget.h"

PositionEditorWidget::PositionEditorWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::PositionEditorWidget)
{
    _ui->setupUi(this);
}

PositionEditorWidget::~PositionEditorWidget()
{
    delete _ui;
}

void PositionEditorWidget::setCurrentPosition(const QPointF &point)
{
    _ui->latitude->setValue(point.y());
    _ui->longitude->setValue(point.x());
}

QPointF PositionEditorWidget::getCurrentPosition() const
{
    return QPointF(_ui->longitude->value(), _ui->latitude->value());
}
