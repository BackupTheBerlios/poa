#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "grid.h"

/** Class to test the Grid class.
*/
class GridTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(GridTest);
    CPPUNIT_TEST(testClosestGridPoint);
    CPPUNIT_TEST(testClosestGridPointX);
    CPPUNIT_TEST(testClosestGridPointY);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testGetGridDistance);
    CPPUNIT_TEST(testMove);
    CPPUNIT_TEST(testMoveInDirection);
    CPPUNIT_TEST_SUITE_END();

private:
    Grid *grid10;
    Grid *grid20;

public:

    void setUp()
    {
        grid10 = new Grid(QPoint(10, 10), 10);
        grid20 = new Grid(QPoint(10, 10), QPoint(30, 30), 20);
    }

    void tearDown()
    {
        delete grid10;
        delete grid20;
    }

    void testConstructor()
    {
        Grid grid(QPoint(10, 10), QPoint(10, 30), 20);
        CPPUNIT_ASSERT(grid.closestGridPoint(QPoint(30, 30)) == QPoint(30, 30));

        grid = Grid(QPoint(10, 10), QPoint(30, 10), 20);
        CPPUNIT_ASSERT(grid.closestGridPoint(QPoint(30, 30)) == QPoint(30, 30));

        grid = Grid(QPoint(10, 10), QPoint(11, 11), 20);
        CPPUNIT_ASSERT(grid.closestGridPoint(QPoint(30, 30)) == QPoint(30, 30));
    }

    void testClosestGridPoint()
    {
        CPPUNIT_ASSERT(grid10->closestGridPoint(QPoint(0, 0)) == QPoint(0, 0));
        CPPUNIT_ASSERT(grid10->closestGridPoint(QPoint(4, 4)) == QPoint(0, 0));
        CPPUNIT_ASSERT(grid10->closestGridPoint(QPoint(-4, -4)) == QPoint(0, 0));
        CPPUNIT_ASSERT(grid10->closestGridPoint(QPoint(5, 5)) == QPoint(0, 0)
                       || grid10->closestGridPoint(QPoint(5, 5)) == QPoint(10, 10));

        CPPUNIT_ASSERT(grid20->closestGridPoint(QPoint(1, 1)) == QPoint(10, 10));
    }

    void testClosestGridPointX()
    {
        CPPUNIT_ASSERT(grid10->closestGridPoint(QPoint(14, 30)) == QPoint(10, 30));
        CPPUNIT_ASSERT(grid10->closestGridPoint(QPoint(-14, 30)) == QPoint(-10, 30));
    }

    void testClosestGridPointY()
    {
        CPPUNIT_ASSERT(grid10->closestGridPoint(QPoint(30, 14)) == QPoint(30, 10));
        CPPUNIT_ASSERT(grid10->closestGridPoint(QPoint(30, -14)) == QPoint(30, -10));
    }

    void testGetGridDistance()
    {
        int x;
        int y;

        grid10->getGridDistance(QPoint(10, 10), QPoint(20, 30), x, y);
        CPPUNIT_ASSERT(x == 1);
        CPPUNIT_ASSERT(y == 2);
    }

    void testMove()
    {
        CPPUNIT_ASSERT(grid10->move(QPoint(6, 6), 0, 0) == QPoint(10, 10));
        CPPUNIT_ASSERT(grid10->move(QPoint(6, 6), -1, -1) == QPoint(0, 0));
        CPPUNIT_ASSERT(grid10->move(QPoint(6, 6), 2, 0) == QPoint(30, 10));
        CPPUNIT_ASSERT(grid10->move(QPoint(6, 6), 0, -2) == QPoint(10, -10));
    }

    void testMoveInDirection()
    {
        CPPUNIT_ASSERT(grid10->move(QPoint(10, 10), LEFT, 1) == QPoint(0, 10));
        CPPUNIT_ASSERT(grid10->move(QPoint(10, 10), RIGHT, 1) == QPoint(20, 10));
        CPPUNIT_ASSERT(grid10->move(QPoint(10, 10), UP, 1) == QPoint(10, 0));
        CPPUNIT_ASSERT(grid10->move(QPoint(10, 10), DOWN, 1) == QPoint(10, 20));
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(GridTest);

