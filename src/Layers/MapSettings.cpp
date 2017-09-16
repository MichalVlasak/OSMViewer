#include "MapSettings.h"
#include "OSMLayer.h"

#ifdef __linux__
#include <cmath>
#else
#define _USE_MATH_DEFINES
#include <math.h>
#endif

MapSettings::MapSettings()
{
    homePosition = QPointF(17.15, 48.15);
}

int MapSettings::long2tilex(double lon, int z)
{
    int retVal = (int)(floor((lon + 180.0) / 360.0 * pow(2.0, z)));
    int maxVal = pow(2.0, z);

    if(retVal == maxVal)
    {
        retVal--;
    }
    if(retVal > maxVal)
    {
        retVal -= (retVal - maxVal) + 1;
    }
    else if(retVal < 0)
    {
        retVal = 0;
    }

    return retVal;
}

int MapSettings::lat2tiley(double lat, int z)
{
    int retVal = (int)(floor((1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z)));
    int maxVal = pow(2.0, z);

    if(retVal == maxVal)
    {
        retVal--;
    }
    if(retVal > maxVal)
    {
        retVal -= (retVal - maxVal) + 1;
    }
    else if(retVal < 0)
    {
        retVal = 0;
    }

    return retVal;
}

double MapSettings::tilex2long(int x, int z)
{
    return x / pow(2.0, z) * 360.0 - 180;
}

double MapSettings::tiley2lat(int y, int z)
{
    double n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
    return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

double MapSettings::tiley2lat2(int y, int z)
{
    double n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
    return 180.0 / M_PI * atan(sin(n));
}

double MapSettings::getPixelForLat(double lat)
{
    double offset = (OSMLayer::TileSize / 2.) * pow(2., zoom.getCurrentZoomLevel());

    return offset + worldCenter.y() - (offset * (1. + log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI));
}

double MapSettings::getPixelForLon(double lon)
{
    double degToPixel =  OSMLayer::TileSize / zoom.getCurrentDegreeForTile();

    return worldCenter.x() + (lon * degToPixel);
}

double MapSettings::getLonForPixelOld(double pix)
{
    double lon = (pix - worldCenter.x()) * zoom.getCurrentDegreeForTile();

    lon /= OSMLayer::TileSize;

    if(lon < -180.)
    {
        lon = -180.;
    }
    else if(lon > 180.)
    {
        lon = 180.;
    }

    return lon;
}

double MapSettings::getLonForPixel(double pix)
{
    double mapSize = pow(2, zoom.getCurrentZoomLevel()) * OSMLayer::TileSize;

    //longitude = Convert.ToSingle(((ClipByRange(pixelX, mapSize - 1) / 256) / Math.Pow(2.0, zoomLevel) * 360.0) - 180.0);
    double longitude = ((fmod(pix, mapSize - 1) / (OSMLayer::TileSize / 2)) / pow(2., zoom.getCurrentZoomLevel()) * 360) - 360.;
    return longitude;
}

double MapSettings::getLatForPixel(double pix)
{
    if(pix < 0)
    {
        return 89.99;
    }

    double mapSize = pow(2, zoom.getCurrentZoomLevel()) * OSMLayer::TileSize;

    if(mapSize < pix)
    {
        return -89.99;
    }

    double n = M_PI - ((2.0 * M_PI * (fmod(pix, mapSize - 1) / OSMLayer::TileSize)) / pow(2., zoom.getCurrentZoomLevel()));
    double latitude = 180. / M_PI * atan(sinh(n));
    return latitude;
}

int MapSettings::windowPixelToMapPixelX(int pix)
{
    int mapSize = int(pow(2, zoom.getCurrentZoomLevel())) * OSMLayer::TileSize;
    return pix - worldCenter.x() + (mapSize / 2);
}

int MapSettings::windowPixelToMapPixelY(int pix)
{
    int mapSize = int(pow(2, zoom.getCurrentZoomLevel())) * OSMLayer::TileSize;
    return pix - worldCenter.y() + (mapSize / 2);
}

/*       C#
internal void LatLongToPixelXYOSM(double latitude, double longitude, int zoomLevel, ref int pixelX, ref int pixelY)
{
        dynamic MinLatitude = -85.05112878;
        dynamic MaxLatitude = 85.05112878;
        dynamic MinLongitude = -180;
        dynamic MaxLongitude = 180;
        dynamic mapSize = Math.Pow(2, zoomLevel) * 256;

        latitude = Clip(latitude, MinLatitude, MaxLatitude);
        longitude = Clip(longitude, MinLongitude, MaxLongitude);

        PointF p = new Point();
        p.X = Convert.ToSingle((longitude + 180.0) / 360.0 * (1 << zoomLevel));
        p.Y = Convert.ToSingle((1.0 - Math.Log(Math.Tan(latitude * Math.PI / 180.0) + 1.0 / Math.Cos(toRadians(latitude))) / Math.PI) / 2.0 * (1 << zoomLevel));

        int tilex = Convert.ToInt32(Math.Truncate(p.X));
        int tiley = Convert.ToInt32(Math.Truncate(p.Y));
        pixelX = ClipByRange((tilex * 256) + ((p.X - tilex) * 256), mapSize - 1);
        pixelY = ClipByRange((tiley * 256) + ((p.Y - tiley) * 256), mapSize - 1);
}

internal void PixelXYToLatLongOSM(int pixelX, int pixelY, int zoomLevel, ref double latitude, ref double longitude)
{
        dynamic mapSize = Math.Pow(2, zoomLevel) * 256;
        int tileX = Math.Truncate(pixelX / 256);
        int tileY = Math.Truncate(pixelY / 256);

        PointF p = new Point();
        double n = Math.PI - ((2.0 * Math.PI * (ClipByRange(pixelY, mapSize - 1) / 256)) / Math.Pow(2.0, zoomLevel));

        longitude = Convert.ToSingle(((ClipByRange(pixelX, mapSize - 1) / 256) / Math.Pow(2.0, zoomLevel) * 360.0) - 180.0);
        latitude = Convert.ToSingle(180.0 / Math.PI * Math.Atan(Math.Sinh(n)));
}

private object ClipByRange(double n, double range)
{
        return n % range;
}

private object Clip(double n, double minValue, double maxValue)
{
        return Math.Min(Math.Max(n, minValue), maxValue);
}

//=======================================================
//Service provided by Telerik (www.telerik.com)
//Conversion powered by NRefactory.
//Twitter: @telerik
//Facebook: facebook.com/telerik
//=======================================================
*/

/*       Visual Basic
Friend Sub LatLongToPixelXYOSM(ByVal latitude As Double, ByVal longitude As Double, ByVal zoomLevel As Integer, ByRef pixelX As Integer, ByRef pixelY As Integer)
        Dim MinLatitude = -85.05112878
        Dim MaxLatitude = 85.05112878
        Dim MinLongitude = -180
        Dim MaxLongitude = 180
        Dim mapSize = Math.Pow(2, zoomLevel) * 256

        latitude = Clip(latitude, MinLatitude, MaxLatitude)
        longitude = Clip(longitude, MinLongitude, MaxLongitude)

        Dim p As PointF = New Point()
        p.X = CSng((longitude + 180.0) / 360.0 * (1 << zoomLevel))
        p.Y = CSng((1.0 - Math.Log(Math.Tan(latitude * Math.PI / 180.0) + 1.0 / Math.Cos(toRadians(latitude))) / Math.PI) / 2.0 * (1 << zoomLevel))

        Dim tilex As Integer = CInt(Math.Truncate(p.X))
        Dim tiley As Integer = CInt(Math.Truncate(p.Y))
        pixelX = ClipByRange((tilex * 256) + ((p.X - tilex) * 256), mapSize - 1)
        pixelY = ClipByRange((tiley * 256) + ((p.Y - tiley) * 256), mapSize - 1)
    End Sub

Friend Sub PixelXYToLatLongOSM(ByVal pixelX As Integer, ByVal pixelY As Integer, ByVal zoomLevel As Integer, ByRef latitude As Double, ByRef longitude As Double)
        Dim mapSize = Math.Pow(2, zoomLevel) * 256
        Dim tileX As Integer = Math.Truncate(pixelX / 256)
        Dim tileY As Integer = Math.Truncate(pixelY / 256)

        Dim p As PointF = New Point()
        Dim n As Double = Math.PI - ((2.0 * Math.PI * (ClipByRange(pixelY, mapSize - 1) / 256)) / Math.Pow(2.0, zoomLevel))

        longitude = CSng(((ClipByRange(pixelX, mapSize - 1) / 256) / Math.Pow(2.0, zoomLevel) * 360.0) - 180.0)
        latitude = CSng(180.0 / Math.PI * Math.Atan(Math.Sinh(n)))
    End Sub

Private Function ClipByRange(ByVal n As Double, ByVal range As Double)
        Return n Mod range
End Function

Private Function Clip(ByVal n As Double, ByVal minValue As Double, ByVal maxValue As Double)
      Return Math.Min(Math.Max(n, minValue), maxValue)
End Function
*/
