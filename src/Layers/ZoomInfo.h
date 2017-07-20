#ifndef ZOOMINFO_H
#define ZOOMINFO_H

#include <QWheelEvent>

class ZoomInfo : public QObject
{
        Q_OBJECT

    public:
        ZoomInfo();

        static const unsigned MaxZoomLevel = 20;

    public:
        void wheelEvent(QWheelEvent * wheelEvent);
        void keyPressEvent(QKeyEvent * keyEvent);

        void zoomIncrement();
        void zoomDecrement();

        void setCurrentZoomLevel(unsigned level);

        unsigned getCurrentZoomLevel();
        unsigned getCurrentScale();
        QString getCurrentScaleString();
        float getCurrentDegreeForTile();

    signals:
        void zoomChanged();

    private:
        struct ZoomStruct
        {
                ZoomStruct(unsigned s, float m, float d)
                {
                    scale = s;
                    meterToPixel = m;
                    degree = d;
                }

                unsigned scale;
                float meterToPixel;
                float degree;
        };

        unsigned _zoomLevel = 0;
        std::vector<ZoomStruct> _zoomScale;
};

#endif // ZOOMINFO_H
