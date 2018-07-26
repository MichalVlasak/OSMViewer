#ifndef POLYGONAREAWIDGET_H
#define POLYGONAREAWIDGET_H

#include "AreaGeometryWigdetInterface.h"
#include "PolygonAreaTableModel.h"

namespace Ui {
class PolygonAreaWidget;
}

class PolygonAreaWidget : public AreaGeometryWigdetInterface
{
        Q_OBJECT

    public:
        explicit PolygonAreaWidget(const AreaGeometry & geometry, QWidget *parent = nullptr);
        ~PolygonAreaWidget();

    public:
        virtual AreaGeometry getGeometry() override;

    private slots:
        void addPoint();
        void deletePoint();
        void editPoint();
        void tableClicked(QModelIndex index);

    private:
        Ui::PolygonAreaWidget * _ui = nullptr;
        PolygonAreaTableModel * _tableModel = nullptr;
        QPolygonF _polygon;
};

#endif // POLYGONAREAWIDGET_H
