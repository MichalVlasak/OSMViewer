#ifndef CIRCLEWIDGET_H
#define CIRCLEWIDGET_H

#include "AreaGeometryWigdetInterface.h"
#include "PolygonAreaTableModel.h"

namespace Ui {
class CircleWidget;
}

class CircleWidget : public AreaGeometryWigdetInterface
{
        Q_OBJECT

    public:
        explicit CircleWidget(const AreaGeometry & geometry, QWidget *parent = nullptr);
        ~CircleWidget();

    public:
        virtual AreaGeometry getGeometry() override;

    private:
        Ui::CircleWidget * _ui = nullptr;
};

#endif // CIRCLEWIDGET_H
