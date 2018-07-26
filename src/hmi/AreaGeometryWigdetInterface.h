#ifndef AREAGEOMETRYWIGDETINTERFACE_H
#define AREAGEOMETRYWIGDETINTERFACE_H

#include <QWidget>

#include "AreaGeometry.h"

class AreaGeometryWigdetInterface : public QWidget
{
    public:
        explicit AreaGeometryWigdetInterface(QWidget *parent = nullptr);
        ~ AreaGeometryWigdetInterface();

        static AreaGeometryWigdetInterface * createGeometryWidget(const AreaGeometry & geometry, QWidget *parent = 0);

    public:
        virtual AreaGeometry getGeometry() = 0;
};

#endif // AREAGEOMETRYWIGDETINTERFACE_H
