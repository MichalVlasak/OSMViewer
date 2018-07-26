#ifndef RECTANGLEAREAWIDGET_H
#define RECTANGLEAREAWIDGET_H

#include "AreaGeometryWigdetInterface.h"

namespace Ui {
class RectangleAreaWidget;
}

class RectangleAreaWidget : public AreaGeometryWigdetInterface
{
        Q_OBJECT

    public:
        RectangleAreaWidget(const AreaGeometry & geometry, QWidget *parent = nullptr);
        ~RectangleAreaWidget();

    public:
        virtual AreaGeometry getGeometry() override;

    private slots:
        void changeLatFrom(double lat);
        void changeLatTo(double lat);

        void changeLonFrom(double lon);
        void changeLonTo(double lon);

    private:
        Ui::RectangleAreaWidget * _ui = nullptr;
        double _lonFrom = 0.;
        double _latFrom = 0.;
        double _lonTo = 0.;
        double _latTo = 0.;
};

#endif // RECTANGLEAREAWIDGET_H
