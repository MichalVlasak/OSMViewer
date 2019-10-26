#include "MapLayersWidget.h"
#include "ui_MapLayersForm.h"

#include <QItemSelectionModel>
#include <iostream>

MapLayersWidget::MapLayersWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::MapLayersForm)
{
    _ui->setupUi(this);

    _standardModel = new QStandardItemModel(this);
    _rootNode = _standardModel->invisibleRootItem();

    _ui->treeView->setModel(_standardModel);
    _ui->treeView->expandAll();

    QObject::connect(_standardModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)));
}

MapLayersWidget::~MapLayersWidget()
{
    delete _ui;
}

void MapLayersWidget::initialize(PaintingWidget * widget)
{
    for(BaseLayer * layer : widget->getLayers())
    {
        createItem(layer, _rootNode);
    }

    _paintingWidget = widget;
}

void MapLayersWidget::createItem(BaseLayer * layer, QStandardItem *parent)
{
    if(layer != nullptr)
    {
        QStandardItem * node = new QStandardItem(layer->getLayerName());

        Qt::CheckState checked = (layer->isVisible() == true) ? Qt::Checked : Qt::Unchecked;

        node->setCheckable(true);
        node->setCheckState(checked);
        node->setData(QVariant::fromValue(layer), LayerDataRole);

        parent->appendRow(node);
    }
}

void MapLayersWidget::itemChanged(QStandardItem *item)
{
    if(item != nullptr)
    {
        std::cout << "changed: " << item->text().toStdString() << " - " << item->checkState() << std::endl;

        QVariant layerVar = item->data(LayerDataRole);

        if(layerVar.isNull() == false && layerVar.canConvert<BaseLayer*>() == true)
        {
            BaseLayer * layer = layerVar.value<BaseLayer*>();

            if(layer != nullptr)
            {
                if(item->checkState() == Qt::Checked || item->checkState() == Qt::PartiallyChecked)
                {
                    layer->setVisible(true);
                }
                else
                {
                    layer->setVisible(false);
                }

                if(_paintingWidget != nullptr)
                {
                    _paintingWidget->repaint();
                }
            }
        }
    }
}
