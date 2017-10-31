#include <QString>
#include <QtTest>

#include "../src/WgsConversion.h"

class WgsToDoubleTest : public QObject
{
        Q_OBJECT

    public:
        WgsToDoubleTest();

    private Q_SLOTS:
        void testCaseSomeString();

        void testCaseNorthEastDeg1();
        void testCaseEastNorthDeg1();
        void testCaseNorthWestDeg1();
        void testCaseWestNorthDeg1();
        void testCaseSouthEastDeg1();
        void testCaseEastSouthDeg1();
        void testCaseSouthWestDeg1();
        void testCaseWestSouthDeg1();

        void testCaseNorthEastDeg2();
        void testCaseEastNorthDeg2();
        void testCaseNorthWestDeg2();
        void testCaseWestNorthDeg2();
        void testCaseSouthEastDeg2();
        void testCaseEastSouthDeg2();
        void testCaseSouthWestDeg2();
        void testCaseWestSouthDeg2();

        void testCaseNorthEastDeg3();
        void testCaseEastNorthDeg3();
        void testCaseNorthWestDeg3();
        void testCaseWestNorthDeg3();
        void testCaseSouthEastDeg3();
        void testCaseEastSouthDeg3();
        void testCaseSouthWestDeg3();
        void testCaseWestSouthDeg3();

        void testCaseNorthEastDegMin();
        void testCaseEastNorthDegMin();
        void testCaseNorthWestDegMin();
        void testCaseWestNorthDegMin();
        void testCaseSouthEastDegMin();
        void testCaseEastSouthDegMin();
        void testCaseSouthWestDegMin();
        void testCaseWestSouthDegMin();

        void testCaseNorthEastDegMinSec1();
        void testCaseEastNorthDegMinSec1();
        void testCaseNorthWestDegMinSec1();
        void testCaseWestNorthDegMinSec1();
        void testCaseSouthEastDegMinSec1();
        void testCaseEastSouthDegMinSec1();
        void testCaseSouthWestDegMinSec1();
        void testCaseWestSouthDegMinSec1();

        void testCaseNorthEastDegMinSec2();
        void testCaseEastNorthDegMinSec2();
        void testCaseNorthWestDegMinSec2();
        void testCaseWestNorthDegMinSec2();
        void testCaseSouthEastDegMinSec2();
        void testCaseEastSouthDegMinSec2();
        void testCaseSouthWestDegMinSec2();
        void testCaseWestSouthDegMinSec2();

        void testCaseDoubleToWgsGps();
};

WgsToDoubleTest::WgsToDoubleTest()
{
}

void WgsToDoubleTest::testCaseSomeString()
{
    QString wgs = "some string";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, false);
}

void WgsToDoubleTest::testCaseNorthEastDeg1()
{
    QString wgs = " 40N 20E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20., lon);
    QCOMPARE(40., lat);
}

void WgsToDoubleTest::testCaseEastNorthDeg1()
{
    QString wgs = " 20E 40N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20., lon);
    QCOMPARE(40., lat);
}

void WgsToDoubleTest::testCaseNorthWestDeg1()
{
    QString wgs = " 40N 20W ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20., lon);
    QCOMPARE(40., lat);
}

void WgsToDoubleTest::testCaseWestNorthDeg1()
{
    QString wgs = " 20W 40N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20., lon);
    QCOMPARE(40., lat);
}

void WgsToDoubleTest::testCaseSouthEastDeg1()
{
    QString wgs = " 40S 20E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20., lon);
    QCOMPARE(-40., lat);
}

void WgsToDoubleTest::testCaseEastSouthDeg1()
{
    QString wgs = " 20E 40S ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20., lon);
    QCOMPARE(-40., lat);
}

void WgsToDoubleTest::testCaseSouthWestDeg1()
{
    QString wgs = " 40S 20W ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20., lon);
    QCOMPARE(-40., lat);
}

void WgsToDoubleTest::testCaseWestSouthDeg1()
{
    QString wgs = " 20W 40S ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20., lon);
    QCOMPARE(-40., lat);
}

void WgsToDoubleTest::testCaseNorthEastDeg2()
{
    QString wgs = " 40ºN 20ºE ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20., lon);
    QCOMPARE(40., lat);
}

void WgsToDoubleTest::testCaseEastNorthDeg2()
{
    QString wgs = " 20ºE 40ºN ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20., lon);
    QCOMPARE(40., lat);
}

void WgsToDoubleTest::testCaseNorthWestDeg2()
{
    QString wgs = " 40ºN 20ºW ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20., lon);
    QCOMPARE(40., lat);
}

void WgsToDoubleTest::testCaseWestNorthDeg2()
{
    QString wgs = " 20ºW 40ºN ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20., lon);
    QCOMPARE(40., lat);
}

void WgsToDoubleTest::testCaseSouthEastDeg2()
{
    QString wgs = " 40ºS 20ºE ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20., lon);
    QCOMPARE(-40., lat);
}

void WgsToDoubleTest::testCaseEastSouthDeg2()
{
    QString wgs = " 20ºE 40ºS ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20., lon);
    QCOMPARE(-40., lat);
}

void WgsToDoubleTest::testCaseSouthWestDeg2()
{
    QString wgs = " 40ºS 20ºW ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20., lon);
    QCOMPARE(-40., lat);
}

void WgsToDoubleTest::testCaseWestSouthDeg2()
{
    QString wgs = " 20ºW 40ºS ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20., lon);
    QCOMPARE(-40., lat);
}

void WgsToDoubleTest::testCaseNorthEastDeg3()
{
    QString wgs = " 40.56N 20.38E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20.38, lon);
    QCOMPARE(40.56, lat);
}

void WgsToDoubleTest::testCaseEastNorthDeg3()
{
    QString wgs = " 20.38E 40.56N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20.38, lon);
    QCOMPARE(40.56, lat);
}

void WgsToDoubleTest::testCaseNorthWestDeg3()
{
    QString wgs = " 40.56N 20.38W ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20.38, lon);
    QCOMPARE(40.56, lat);
}

void WgsToDoubleTest::testCaseWestNorthDeg3()
{
    QString wgs = " 20.38W 40.56N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20.38, lon);
    QCOMPARE(40.56, lat);
}

void WgsToDoubleTest::testCaseSouthEastDeg3()
{
    QString wgs = " 40.56S 20.38E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20.38, lon);
    QCOMPARE(-40.56, lat);
}

void WgsToDoubleTest::testCaseEastSouthDeg3()
{
    QString wgs = " 20.38E 40.56S ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20.38, lon);
    QCOMPARE(-40.56, lat);
}

void WgsToDoubleTest::testCaseSouthWestDeg3()
{
    QString wgs = " 40.56S 20.38W ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20.38, lon);
    QCOMPARE(-40.56, lat);
}

void WgsToDoubleTest::testCaseWestSouthDeg3()
{
    QString wgs = " 20.38W 40.56S ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20.38, lon);
    QCOMPARE(-40.56, lat);
}

void WgsToDoubleTest::testCaseNorthEastDegMin()
{
    QString wgs = " 40º 15' N 20º 45' E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20.75, lon);
    QCOMPARE(40.25, lat);
}

void WgsToDoubleTest::testCaseEastNorthDegMin()
{
    QString wgs = " 20º 45' E 40º 15' N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20.75, lon);
    QCOMPARE(40.25, lat);
}

void WgsToDoubleTest::testCaseNorthWestDegMin()
{
    QString wgs = " 40º 15' N 20º 45' W ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20.75, lon);
    QCOMPARE(40.25, lat);
}

void WgsToDoubleTest::testCaseWestNorthDegMin()
{
    QString wgs = " 20º 45' W 40º 15' N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20.75, lon);
    QCOMPARE(40.25, lat);
}

void WgsToDoubleTest::testCaseSouthEastDegMin()
{
    QString wgs = " 40º 15' S 20º 45' E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20.75, lon);
    QCOMPARE(-40.25, lat);
}

void WgsToDoubleTest::testCaseEastSouthDegMin()
{
    QString wgs = " 20º 45' E 40º 15' S ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20.75, lon);
    QCOMPARE(-40.25, lat);
}

void WgsToDoubleTest::testCaseSouthWestDegMin()
{
    QString wgs = " 40º 15' S 20º 45' W ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20.75, lon);
    QCOMPARE(-40.25, lat);
}

void WgsToDoubleTest::testCaseWestSouthDegMin()
{
    QString wgs = " 20º 45' W 40º 15' S ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(-20.75, lon);
    QCOMPARE(-40.25, lat);
}

void WgsToDoubleTest::testCaseNorthEastDegMinSec1()
{
    QString wgs = " 40º 15' 30\" N 20º 45' 30\" E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(((30./60. + 45.) / 60.) + 20., lon);
    QCOMPARE(((30./60. + 15.) / 60.) + 40., lat);
}

void WgsToDoubleTest::testCaseEastNorthDegMinSec1()
{
    QString wgs = " 20º 45' 30\" E 40º 15' 30\" N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(((30./60. + 45.) / 60.) + 20., lon);
    QCOMPARE(((30./60. + 15.) / 60.) + 40., lat);
}

void WgsToDoubleTest::testCaseNorthWestDegMinSec1()
{
    QString wgs = " 40º 15' 30\" N 20º 45' 30\" W ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE((((30./60. + 45.) / 60.) + 20.) * -1., lon);
    QCOMPARE(((30./60. + 15.) / 60.) + 40., lat);
}

void WgsToDoubleTest::testCaseWestNorthDegMinSec1()
{
    QString wgs = " 20º 45' 30\" W 40º 15' 30\" N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE((((30./60. + 45.) / 60.) + 20.) * -1., lon);
    QCOMPARE(((30./60. + 15.) / 60.) + 40., lat);
}

void WgsToDoubleTest::testCaseSouthEastDegMinSec1()
{
    QString wgs = " 40º 15' 30\" S 20º 45' 30\" E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE((((30./60. + 45.) / 60.) + 20.) * 1., lon);
    QCOMPARE((((30./60. + 15.) / 60.) + 40.) * -1., lat);
}

void WgsToDoubleTest::testCaseEastSouthDegMinSec1()
{
    QString wgs = " 20º 45' 30\" E 40º 15' 30\" S ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE((((30./60. + 45.) / 60.) + 20.) * 1., lon);
    QCOMPARE((((30./60. + 15.) / 60.) + 40.) * -1., lat);
}

void WgsToDoubleTest::testCaseSouthWestDegMinSec1()
{
    QString wgs = " 40º 15' 30\" S 20º 45' 30\" W ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE((((30./60. + 45.) / 60.) + 20.) * -1., lon);
    QCOMPARE((((30./60. + 15.) / 60.) + 40.) * -1., lat);
}

void WgsToDoubleTest::testCaseWestSouthDegMinSec1()
{
    QString wgs = " 20º 45' 30\" W 40º 15' 30\" S ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE((((30./60. + 45.) / 60.) + 20.) * -1., lon);
    QCOMPARE((((30./60. + 15.) / 60.) + 40.) * -1., lat);
}

void WgsToDoubleTest::testCaseNorthEastDegMinSec2()
{
    QString wgs = " 40º 15' 30.125\" N 20º 45' 30.236\" E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(((30.236/60. + 45.) / 60.) + 20., lon);
    QCOMPARE(((30.125/60. + 15.) / 60.) + 40., lat);
}

void WgsToDoubleTest::testCaseEastNorthDegMinSec2()
{
    QString wgs = " 20º 45' 30.236\" E 40º 15' 30.125\" N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(((30.236/60. + 45.) / 60.) + 20., lon);
    QCOMPARE(((30.125/60. + 15.) / 60.) + 40., lat);
}

void WgsToDoubleTest::testCaseNorthWestDegMinSec2()
{
    QString wgs = " 40º 15' 30.125\" N 20º 45' 30.236\" W ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE((((30.236/60. + 45.) / 60.) + 20.) * -1., lon);
    QCOMPARE(((30.125/60. + 15.) / 60.) + 40., lat);
}

void WgsToDoubleTest::testCaseWestNorthDegMinSec2()
{
    QString wgs = " 20º 45' 30.236\" W 40º 15' 30.125\" N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE((((30.236/60. + 45.) / 60.) + 20.) * -1., lon);
    QCOMPARE(((30.125/60. + 15.) / 60.) + 40., lat);
}

void WgsToDoubleTest::testCaseSouthEastDegMinSec2()
{
    QString wgs = " 40º 15' 30.125\" S 20º 45' 30.236\" E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE((((30.236/60. + 45.) / 60.) + 20.) * 1., lon);
    QCOMPARE((((30.125/60. + 15.) / 60.) + 40.) * -1., lat);
}

void WgsToDoubleTest::testCaseEastSouthDegMinSec2()
{
    QString wgs = " 20º 45' 30.236\" E 40º 15' 30.125\" S ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE((((30.236/60. + 45.) / 60.) + 20.) * 1., lon);
    QCOMPARE((((30.125/60. + 15.) / 60.) + 40.) * -1., lat);
}

void WgsToDoubleTest::testCaseSouthWestDegMinSec2()
{
    QString wgs = " 40º 15' 30.125\" S 20º 45' 30.236\" W ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE((((30.236/60. + 45.) / 60.) + 20.) * -1., lon);
    QCOMPARE((((30.125/60. + 15.) / 60.) + 40.) * -1., lat);
}

void WgsToDoubleTest::testCaseWestSouthDegMinSec2()
{
    QString wgs = " 20º 45' 30.236\" W 40º 15' 30.125\" S ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE((((30.236/60. + 45.) / 60.) + 20.) * -1., lon);
    QCOMPARE((((30.125/60. + 15.) / 60.) + 40.) * -1., lat);
}

void WgsToDoubleTest::testCaseDoubleToWgsGps()
{
    double lat = 45.25;
    double lon = -30.75;
    QString wgsLon = WgsConversion::convertDoubleDegToWgs(lon, WgsConversion::Longitude, true, true);
    QString wgsLat = WgsConversion::convertDoubleDegToWgs(lat, WgsConversion::Latitude, true, true);

    QVERIFY(wgsLat == "45º 15' 00.000\" N");
    QVERIFY(wgsLon == "30º 45' 00.000\" W");
}

QTEST_APPLESS_MAIN(WgsToDoubleTest)

#include "tst_WgsToDoubleTest.moc"
