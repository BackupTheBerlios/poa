#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include <qpoint.h>
#include <qrect.h>

#include "poa.h"

/** Class to test the poa.cpp.
*/
class PoaTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(PoaTest);
    CPPUNIT_TEST(testImageQPoint);
    CPPUNIT_TEST(testImageQRect);
    CPPUNIT_TEST(testLineDirection);
    CPPUNIT_TEST(testDistInDir);
    CPPUNIT_TEST(testAlternateDir);
    CPPUNIT_TEST(testRightAngle);
    CPPUNIT_TEST(testIsTurn);
    CPPUNIT_TEST(testTurnLeft);
    CPPUNIT_TEST(testTurnRight);
    CPPUNIT_TEST(testImageDir);
    CPPUNIT_TEST(testFormatTime);
    CPPUNIT_TEST_SUITE_END();

public:

    void setUp()
    {
    }

    void tearDown()
    {
    }


    void testImageQPoint()
    {
        QPoint point(5,5);
        QPoint pointn(-5,5);

        CPPUNIT_ASSERT(image(point) == "(5, 5)");
        CPPUNIT_ASSERT(image(pointn) == "(-5, 5)");
    }

    void testImageQRect()
    {
        CPPUNIT_ASSERT(image(QRect(5, 5, 5, 5)) == "((5, 5), (9, 9))");
    }

    void testLineDirection()
    {
        CPPUNIT_ASSERT(reverse(LEFT) == RIGHT);
        CPPUNIT_ASSERT(reverse(RIGHT) == LEFT);
        CPPUNIT_ASSERT(reverse(UP) == DOWN);
        CPPUNIT_ASSERT(reverse(DOWN) == UP);
//          CPPUNIT_ASSERT(reverse(UNKNOWN) == UNKNOWN);
    }

    void testDistInDir()
    {
        int x = 10;
        int y = 20;
        CPPUNIT_ASSERT(distInDir(LEFT, x, y) == -x);
        CPPUNIT_ASSERT(distInDir(RIGHT, x, y) == x);
        CPPUNIT_ASSERT(distInDir(UP, x, y) == -y);
        CPPUNIT_ASSERT(distInDir(DOWN, x, y) == y);
//          CPPUNIT_ASSERT(distInDir(UNKNOWN, x, y) == 0);
    }

    void testAlternateDir()
    {
        /*CPPUNIT_ASSERT(distInDir(LEFT, 10, 20) == DOWN);
        CPPUNIT_ASSERT(distInDir(RIGHT,10, 20) == DOWN);
        CPPUNIT_ASSERT(distInDir(LEFT, 10, 0) == UP);
        CPPUNIT_ASSERT(distInDir(RIGHT,10, 0) == UP);
        CPPUNIT_ASSERT(distInDir(LEFT, 10, -20) == UP);
        CPPUNIT_ASSERT(distInDir(RIGHT,10, -20) == UP);*/


    }

    void testRightAngle()
    {
        CPPUNIT_ASSERT(isRightAngle(LEFT, UP));
        CPPUNIT_ASSERT(isRightAngle(LEFT, DOWN));
        CPPUNIT_ASSERT(!isRightAngle(LEFT, LEFT));
        CPPUNIT_ASSERT(!isRightAngle(LEFT, RIGHT));

        CPPUNIT_ASSERT(isRightAngle(RIGHT, UP));
        CPPUNIT_ASSERT(isRightAngle(RIGHT, DOWN));
        CPPUNIT_ASSERT(!isRightAngle(RIGHT, LEFT));
        CPPUNIT_ASSERT(!isRightAngle(RIGHT, RIGHT));

        CPPUNIT_ASSERT(!isRightAngle(UP, UP));
        CPPUNIT_ASSERT(!isRightAngle(UP, DOWN));
        CPPUNIT_ASSERT(isRightAngle(UP, LEFT));
        CPPUNIT_ASSERT(isRightAngle(UP, RIGHT));

        CPPUNIT_ASSERT(!isRightAngle(DOWN, UP));
        CPPUNIT_ASSERT(!isRightAngle(DOWN, DOWN));
        CPPUNIT_ASSERT(isRightAngle(DOWN, LEFT));
        CPPUNIT_ASSERT(isRightAngle(DOWN, RIGHT));

//          CPPUNIT_ASSERT(!isRightAngle(UNKNOWN, UP));
//          CPPUNIT_ASSERT(!isRightAngle(UNKNOWN, DOWN));
//          CPPUNIT_ASSERT(!isRightAngle(UNKNOWN, LEFT));
//          CPPUNIT_ASSERT(!isRightAngle(UNKNOWN, RIGHT));

//          CPPUNIT_ASSERT(!isRightAngle(LEFT, UNKNOWN));
//          CPPUNIT_ASSERT(!isRightAngle(RIGHT, UNKNOWN));
//          CPPUNIT_ASSERT(!isRightAngle(UP, UNKNOWN));
//          CPPUNIT_ASSERT(!isRightAngle(DOWN, UNKNOWN));

//          CPPUNIT_ASSERT(!isRightAngle(UNKNOWN, UNKNOWN));
    }

    void testIsTurn()
    {
//          CPPUNIT_ASSERT(!isTurn(UNKNOWN, UNKNOWN));

//          CPPUNIT_ASSERT(!isTurn(UNKNOWN, LEFT));
//          CPPUNIT_ASSERT(!isTurn(UNKNOWN, RIGHT));
//          CPPUNIT_ASSERT(!isTurn(UNKNOWN, UP));
//          CPPUNIT_ASSERT(!isTurn(UNKNOWN, DOWN));

        CPPUNIT_ASSERT(!isTurn(LEFT, LEFT));
        CPPUNIT_ASSERT(isTurn(LEFT, RIGHT));
        CPPUNIT_ASSERT(!isTurn(LEFT, UP));
        CPPUNIT_ASSERT(!isTurn(LEFT, DOWN));

        CPPUNIT_ASSERT(isTurn(RIGHT, LEFT));
        CPPUNIT_ASSERT(!isTurn(RIGHT, RIGHT));
        CPPUNIT_ASSERT(!isTurn(RIGHT, UP));
        CPPUNIT_ASSERT(!isTurn(RIGHT, DOWN));

        CPPUNIT_ASSERT(!isTurn(UP, LEFT));
        CPPUNIT_ASSERT(!isTurn(UP, RIGHT));
        CPPUNIT_ASSERT(!isTurn(UP, UP));
        CPPUNIT_ASSERT(isTurn(UP, DOWN));

        CPPUNIT_ASSERT(!isTurn(DOWN, LEFT));
        CPPUNIT_ASSERT(!isTurn(DOWN, RIGHT));
        CPPUNIT_ASSERT(isTurn(DOWN, UP));
        CPPUNIT_ASSERT(!isTurn(DOWN, DOWN));
    }


    void testTurnLeft()
    {
//          CPPUNIT_ASSERT(turnLeft(UNKNOWN) == UNKNOWN);

        CPPUNIT_ASSERT(turnLeft(LEFT) == DOWN);
        CPPUNIT_ASSERT(turnLeft(RIGHT) == UP);
        CPPUNIT_ASSERT(turnLeft(UP) == LEFT);
        CPPUNIT_ASSERT(turnLeft(DOWN) == RIGHT);
    }

    void testTurnRight()
    {
//          CPPUNIT_ASSERT(turnRight(UNKNOWN) == UNKNOWN);

        CPPUNIT_ASSERT(turnRight(LEFT) == UP);
        CPPUNIT_ASSERT(turnRight(RIGHT) == DOWN);
        CPPUNIT_ASSERT(turnRight(UP) == RIGHT);
        CPPUNIT_ASSERT(turnRight(DOWN) == LEFT);
    }

    void testImageDir()
    {
//          CPPUNIT_ASSERT(image(UNKNOWN) == 0);

        CPPUNIT_ASSERT(image(LEFT) == "left");
        CPPUNIT_ASSERT(image(RIGHT) == "right");
        CPPUNIT_ASSERT(image(UP) == "up");
        CPPUNIT_ASSERT(image(DOWN) == "down");
    }

    void testFormatTime()
    {
        CPPUNIT_ASSERT(formatTimeProperly(1) == "1 ns");
        CPPUNIT_ASSERT(formatTimeProperly(10) == "10 ns");
        CPPUNIT_ASSERT(formatTimeProperly(100) == "100 ns");
        CPPUNIT_ASSERT(formatTimeProperly(1000) == "1 \u00B5s");
        CPPUNIT_ASSERT(formatTimeProperly(10000) == "10 \u00B5s");
        CPPUNIT_ASSERT(formatTimeProperly(100000) == "100 \u00B5s");
        CPPUNIT_ASSERT(formatTimeProperly(1000000) == "1 ms");
        CPPUNIT_ASSERT(formatTimeProperly(10000000) == "10 ms");
        CPPUNIT_ASSERT(formatTimeProperly(100000000) == "100 ms");
        CPPUNIT_ASSERT(formatTimeProperly(1000000000) == "1 s");

        CPPUNIT_ASSERT(formatTimeProperly(1) == "1 ns");
        CPPUNIT_ASSERT(formatTimeProperly(15) == "15 ns");
        CPPUNIT_ASSERT(formatTimeProperly(105) == "105 ns");
        CPPUNIT_ASSERT(formatTimeProperly(1005) == "1.005 \u00B5s");
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(PoaTest);
