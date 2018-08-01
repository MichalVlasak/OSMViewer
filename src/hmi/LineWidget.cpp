#include "LineWidget.h"
#include "ui_LineWidget.h"
#include "PointEditorDialog.h"

#include <QSpinBox>

LineWidget::LineWidget(const AreaGeometry & geometry, QWidget *parent)
    : AreaGeometryWigdetInterface(parent),
      _ui(new Ui::LineWidget)
{
    _ui->setupUi(this);

    if(geometry.geometryType == AreaGeometry::Type::Line &&
       geometry.geometry.isNull() == false &&
       geometry.geometry.canConvert<QPolygonF>() == true)
    {
        _polygon = geometry.geometry.value<QPolygonF>();

        _tableModel = new PolygonAreaTableModel(this);

        _ui->tableView->setModel(_tableModel);

        _tableModel->setPolygon(_polygon);
    }

    QObject::connect(_ui->addAfterBtn, SIGNAL(clicked(bool)), SLOT(addPoint()));
    QObject::connect(_ui->deleteBtn, SIGNAL(clicked(bool)), SLOT(deletePoint()));
    QObject::connect(_ui->editBtn, SIGNAL(clicked(bool)), SLOT(editPoint()));
    QObject::connect(_ui->tableView, SIGNAL(doubleClicked(QModelIndex)), SLOT(editPoint()));
    QObject::connect(_ui->tableView, SIGNAL(clicked(QModelIndex)), SLOT(tableClicked(QModelIndex)));

    _ui->addAfterBtn->setDisabled(true);
    _ui->deleteBtn->setDisabled(true);
    _ui->editBtn->setDisabled(true);
}

LineWidget::~LineWidget()
{
    delete _ui;
}

AreaGeometry LineWidget::getGeometry()
{
    AreaGeometry geom;
    AreaGeometry::LineBufferGeometry line;

    line.line = _polygon;
    line.bufferWidth = _ui->bufferWidth->value();

    geom.geometryType = AreaGeometry::Type::Line;
    geom.geometry = QVariant::fromValue(line);

    return geom;
}

void LineWidget::tableClicked(QModelIndex index)
{
    _ui->deleteBtn->setDisabled(_polygon.size() < 5);
    _ui->addAfterBtn->setDisabled(false);

    if(index.isValid())
    {
        int row = index.row();

        if(row > -1 && row < _polygon.size())
        {
            _ui->deleteBtn->setDisabled(false);
            _ui->editBtn->setDisabled(false);
        }
    }
}

void LineWidget::addPoint()
{
    QModelIndexList indexes = _ui->tableView->selectionModel()->selectedIndexes();

    if(indexes.size() > 0)
    {
        QModelIndex index = indexes.at(0);

        if(index.isValid())
        {
            int row = index.row();

            if(row > -1 && row < _polygon.size())
            {
                PointEditorDialog * editor = new PointEditorDialog(_polygon[row], this);

                if(editor->exec() == QDialog::Accepted)
                {
                    _polygon.push_back(_polygon[_polygon.size() - 1]);

                    for(int i = _polygon.size() - 2; i > row; i--)
                    {
                        _polygon[i] = _polygon[i - 1];
                    }

                    _polygon[row + 1] = editor->getCurrentPosition();

                    _tableModel->setPolygon(_polygon);
                }
            }
        }
    }
}

void LineWidget::deletePoint()
{
    QModelIndexList indexes = _ui->tableView->selectionModel()->selectedIndexes();

    if(indexes.size() > 0)
    {
        QModelIndex index = indexes.at(0);

        if(index.isValid())
        {
            int row = index.row();

            if(row > -1 && _polygon.size() > row)
            {
                _polygon.remove(row);

                if(row == 0)
                {
                    _polygon[_polygon.size() - 1] = _polygon[0];
                }

                if(_tableModel != nullptr)
                {
                    _tableModel->setPolygon(_polygon);
                }

                _ui->deleteBtn->setDisabled(true);
                _ui->addAfterBtn->setDisabled(true);
            }
        }
    }
}

void LineWidget::editPoint()
{
    QModelIndexList indexes = _ui->tableView->selectionModel()->selectedIndexes();

    if(indexes.size() > 0)
    {
        QModelIndex index = indexes.at(0);

        if(index.isValid())
        {
            int row = index.row();

            if(row > -1 && _polygon.size() > row)
            {
                PointEditorDialog * editor = new PointEditorDialog(_polygon[row], this);

                if(editor->exec() == QDialog::Accepted)
                {
                    _polygon[row] = editor->getCurrentPosition();

                    if(row == 0)
                    {
                        _polygon[_polygon.size() - 1] = _polygon[0];
                    }
                    else if(_polygon.size() == (row + 1))
                    {
                        _polygon[0] = _polygon[_polygon.size() - 1];
                    }

                    _tableModel->setPolygon(_polygon);
                }
            }
        }
    }
}
