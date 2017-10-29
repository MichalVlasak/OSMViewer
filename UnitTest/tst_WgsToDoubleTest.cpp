#include <QString>
#include <QtTest>

#include "../src/WgsConversion.h"

class WgsToDoubleTest : public QObject
{
        Q_OBJECT

    public:
        WgsToDoubleTest();

    private Q_SLOTS:
        void testCase0();
        void testCase1();
        void testCase2();
        void testCase3();
        void testCase4();
        void testCase5();
        void testCase6();
        void testCase7();
        void testCase8();
        void testCase9();
        void testCase10();
        void testCase11();
        void testCase12();
        void testCase13();
};

WgsToDoubleTest::WgsToDoubleTest()
{
}

void WgsToDoubleTest::testCase0()
{
    QString wgs = "some string";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, false);
}

void WgsToDoubleTest::testCase1()
{
    QString wgs = " 40N 20E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20., lon);
    QCOMPARE(40., lat);
}

void WgsToDoubleTest::testCase2()
{
    QString wgs = " 20E 40N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20., lon);
    QCOMPARE(40., lat);
}

void WgsToDoubleTest::testCase3()
{
    QString wgs = " 40.56N 20.38E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20.38, lon);
    QCOMPARE(40.56, lat);
}

void WgsToDoubleTest::testCase4()
{
    QString wgs = " 20.38E 40.56N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20.38, lon);
    QCOMPARE(40.56, lat);
}

void WgsToDoubleTest::testCase5()
{
    QString wgs = " 40º 15' N 20º 45' E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20.75, lon);
    QCOMPARE(40.25, lat);
}

void WgsToDoubleTest::testCase6()
{
    QString wgs = " 20º 45' E 40º 15' N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20.75, lon);
    QCOMPARE(40.25, lat);
}

void WgsToDoubleTest::testCase7()
{
    QString wgs = " 40º 15' 30\" N 20º 45' 30\" E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(((30./60. + 45.) / 60.) + 20., lon);
    QCOMPARE(((30./60. + 15.) / 60.) + 40., lat);
}

void WgsToDoubleTest::testCase8()
{
    QString wgs = " 20º 45' 30\" E 40º 15' 30\" N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(((30./60. + 45.) / 60.) + 20., lon);
    QCOMPARE(((30./60. + 15.) / 60.) + 40., lat);
}

void WgsToDoubleTest::testCase9()
{
    QString wgs = " 40º 15' 30.125\" N 20º 45' 30.236\" E ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(((30.236/60. + 45.) / 60.) + 20., lon);
    QCOMPARE(((30.125/60. + 15.) / 60.) + 40., lat);
}

void WgsToDoubleTest::testCase10()
{
    QString wgs = " 20º 45' 30.236\" E 40º 15' 30.125\" N ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(((30.236/60. + 45.) / 60.) + 20., lon);
    QCOMPARE(((30.125/60. + 15.) / 60.) + 40., lat);
}

void WgsToDoubleTest::testCase11()
{
    double lat = 45.25;
    double lon = -30.75;
    QString wgsLon = WgsConversion::convertDoubleDegToWgs(lon, WgsConversion::Longitude, true, true);
    QString wgsLat = WgsConversion::convertDoubleDegToWgs(lat, WgsConversion::Latitude, true, true);

    QVERIFY(wgsLat == "45º 15' 00.000\" N");
    QVERIFY(wgsLon == "30º 45' 00.000\" W");
}

void WgsToDoubleTest::testCase12()
{
    QString wgs = " 40ºN 20ºE ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20., lon);
    QCOMPARE(40., lat);
}

void WgsToDoubleTest::testCase13()
{
    QString wgs = " 20ºE 40ºN ";
    double lat = 0.;
    double lon = 0.;
    bool result = WgsConversion::convertWgsToDouble(wgs, lon, lat);

    QCOMPARE(result, true);
    QCOMPARE(20., lon);
    QCOMPARE(40., lat);
}

QTEST_APPLESS_MAIN(WgsToDoubleTest)

#include "tst_WgsToDoubleTest.moc"
