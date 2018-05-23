#ifndef GRIDLAYER_H
#define GRIDLAYER_H

#include "BaseLayer.h"

class GridLayer : public BaseLayer
{
    public:
        explicit GridLayer(MapSettings & mapSettings);

    public:
        void paintEvent(QPainter & painter);

    private:
        double DegreeUnit = 360 * 60 * 60; // pocet sekund v celom kruhu

        struct GridLevelInfo
        {
                GridLevelInfo(QColor c, float w, float s, int l, bool p)
                {
                    color = c;
                    lineWidth = w;
                    gridScale = s;
                    level = l;
                    isPrinted = p;
                }

                QColor color;
                float lineWidth;
                double gridScale;
                int level;
                bool isPrinted;
        };
        typedef std::vector<GridLevelInfo> GridLevels;
        GridLevels _gridLevel;

        void paintGrid(GridLevelInfo levelInfo, QPainter & painter);
};

#endif // GRIDLAYER_H
