#ifndef LINEWIDGET_H
#define LINEWIDGET_H

#include "AreaGeometryWigdetInterface.h"
#include "PolygonAreaTableModel.h"

namespace Ui {
class LineWidget;
}

class LineWidget : public AreaGeometryWigdetInterface
{
        Q_OBJECT

    public:
        explicit LineWidget(const AreaGeometry & geometry, QWidget *parent = nullptr);
        ~LineWidget();

    public:
        virtual AreaGeometry getGeometry() override;

    private slots:
        void addPoint();
        void deletePoint();
        void editPoint();
        void tableClicked(QModelIndex index);

    private:
        Ui::LineWidget * _ui = nullptr;
        PolygonAreaTableModel * _tableModel = nullptr;
        QPolygonF _polygon;
};

#endif // LINEWIDGET_H
