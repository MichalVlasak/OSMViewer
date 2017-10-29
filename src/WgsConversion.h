#ifndef WGSCONVERSION_H
#define WGSCONVERSION_H

#include <QString>

class WgsConversion
{
    public:
        enum LatLonInfo
        {
            Latitude, Longitude
        };

        static bool convertWgsToDouble(const QString & wgs, double & lon, double &lat);
        static QString convertDoubleSecToWgs(double degree, LatLonInfo latLon, bool showFullNumber = false, bool showSecDecimals = false);
        static QString convertDoubleDegToWgs(double degree, LatLonInfo latLon, bool showFullNumber = false, bool showSecDecimals = false);

    private:
        enum Hemi
        {
            Unknown, North, South, West, East
        };

    private:
        static bool getNextNumber(QString & wgs, double & retVal);
        static bool getHemi(QString & wgs, Hemi & hemi);
        static void setlatLonValue(double & value, Hemi & hemi, double &lon, double &lat);
};

#endif // WGSCONVERSION_H
