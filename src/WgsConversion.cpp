#include "WgsConversion.h"

#include <cmath>

bool WgsConversion::getNextNumber(QString & wgs, double & retVal)
{
    wgs = wgs.trimmed();

    if(wgs.size() > 0)
    {
        QString tmp;
        bool isSomeNumber = false;

        for(int i = 0; i < wgs.size(); i++)
        {
            if(isdigit(wgs.at(i).toLatin1()) || wgs.at(i) == '.' || wgs.at(i) == ',')
            {
                tmp.push_back(wgs[i]);
                isSomeNumber = true;
            }
            else
            {
                wgs.remove(0, i);
                wgs = wgs.trimmed();

                break;
            }
        }

        if(isSomeNumber == true)
        {
            retVal = tmp.toDouble();

            return true;
        }

        return false;
    }

    return false;
}

bool WgsConversion::getHemi(QString & wgs, Hemi & hemi)
{
    wgs = wgs.trimmed();

    if(wgs.size() > 0)
    {
        switch (wgs.at(0).toLatin1())
        {
            case 'N':
                hemi = North;
                break;

            case 'S':
                hemi = South;
                break;

            case 'E':
                hemi = East;
                break;

            case 'W':
                hemi = West;
                break;

            default:
                hemi = Unknown;
                return false;
        }

        wgs.remove(0, 1);
        wgs = wgs.trimmed();

        return true;
    }

    return false;
}

void WgsConversion::setlatLonValue(double & value, Hemi & hemi, double &lon, double &lat)
{
    switch (hemi)
    {
        case North:
            lat = value;
            break;

        case South:
            lat = value * -1.;
            break;

        case East:
            lon = value;
            break;

        case West:
            lon = value * -1.;
            break;

        default:
            break;
    }
}

bool WgsConversion::convertWgsToDouble(const QString & wgs, double &lon, double &lat)
{
    double num = 0;
    QString wgsTmp = wgs;

    wgsTmp = wgsTmp.trimmed();

    if(getNextNumber(wgsTmp, num) == true)
    {
        Hemi hemi = Unknown;

        if(getHemi(wgsTmp, hemi) == true)
        {
            setlatLonValue(num, hemi, lon, lat);

            convertWgsToDouble(wgsTmp, lon, lat);

            return true;
        }
        else
        {
            if(wgsTmp.size() > 0 && wgsTmp[0] == "º")
            {
                double degVal = num;

                wgsTmp.remove(0, 1);
                wgsTmp = wgsTmp.trimmed();

                if(getHemi(wgsTmp, hemi) == true)
                {
                    setlatLonValue(num, hemi, lon, lat);

                    convertWgsToDouble(wgsTmp, lon, lat);

                    return true;
                }
                else if(getNextNumber(wgsTmp, num) == true)
                {
                    if(wgsTmp.size() > 0 && wgsTmp[0] == "'")
                    {
                        double minVal = num;

                        wgsTmp.remove(0, 1);
                        wgsTmp = wgsTmp.trimmed();

                        hemi = Unknown;

                        if(getHemi(wgsTmp, hemi) == true)
                        {
                            num = degVal + minVal / 60.;
                            setlatLonValue(num, hemi, lon, lat);

                            convertWgsToDouble(wgsTmp, lon, lat);

                            return true;
                        }
                        else
                        {
                            if(getNextNumber(wgsTmp, num) == true)
                            {

                                double secVal = num;

                                wgsTmp.remove(0, 1);
                                wgsTmp = wgsTmp.trimmed();

                                hemi = Unknown;

                                if(getHemi(wgsTmp, hemi) == true)
                                {
                                    num = degVal + (minVal + (secVal / 60.)) / 60.;
                                    setlatLonValue(num, hemi, lon, lat);

                                    convertWgsToDouble(wgsTmp, lon, lat);

                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

QString WgsConversion::convertDoubleSecToWgs(double degree, LatLonInfo latLon, bool showFullNumber, bool showSecDecimals)
{
    QString retval;
    QString hemi;

    if(degree < 0)
    {
        if(latLon == Latitude)
        {
            hemi = "S";
        }
        else
        {
            hemi = "W";
        }
    }
    else
    {
        if(latLon == Latitude)
        {
            hemi = "N";
        }
        else
        {
            hemi = "E";
        }
    }

    degree = fabs(degree);
    int deg = int(degree / (60. * 60.));

    degree -= deg * 60. * 60.;
    int min = int(degree / 60.);

    degree -= min * 60.;
    int sec = int(degree);

    double secDec = degree;

    retval = QString::number(deg) + "º ";

    if(min != 0)
    {
        retval += ((min < 10) ? "0" : "") + QString::number(min) + "' ";
    }

    if(sec != 0 || showFullNumber == true)
    {
        if(min == 0)
        {
            retval += "00'";
        }

        QString secString;

        if(showSecDecimals == true)
        {
            secString = QString::number(secDec, 'f', 3);
        }
        else
        {
            secString = QString::number(sec);
        }

        retval += ((sec < 10) ? "0" : "") + secString + "\" ";
    }

    retval += hemi;

    return retval;
}

QString WgsConversion::convertDoubleDegToWgs(double degree, LatLonInfo latLon, bool showFullNumber, bool showSecDecimals)
{
    return convertDoubleSecToWgs(degree * 60. * 60., latLon, showFullNumber, showSecDecimals);
}
