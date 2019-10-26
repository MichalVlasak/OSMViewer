#ifndef MAPLAYERSFORM_H
#define MAPLAYERSFORM_H

#include <QWidget>
#include <QStandardItemModel>

#include "../Layers/PaintingWidget.h"

namespace Ui {
class MapLayersForm;
}

class MapLayersWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit MapLayersWidget(QWidget *parent = nullptr);
        ~MapLayersWidget();

        void initialize(PaintingWidget * widget);

    private slots:
        void itemChanged(QStandardItem *item);

    private:
        void createItem(BaseLayer * layer, QStandardItem * parent);

    private:
        Ui::MapLayersForm * _ui = nullptr;
        QStandardItemModel * _standardModel = nullptr;
        QStandardItem * _rootNode = nullptr;
        const int LayerDataRole = Qt::UserRole + 1;
        PaintingWidget * _paintingWidget = nullptr;
};

#endif // MAPLAYERSFORM_H
