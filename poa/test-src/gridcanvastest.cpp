#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "gridcanvas.h"
#include "settings.h"

#include <qcanvas.h>

/** Class to test the GridCanvas class.
*/
class GridCanvasTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(GridCanvasTest);
    CPPUNIT_TEST(testEnsureVisibility);
    CPPUNIT_TEST(testToGrid);
    CPPUNIT_TEST_SUITE_END();

private:
    GridCanvas *canvas;

public:

    void setUp()
    {
        canvas = new GridCanvas("Test");
    }

    void tearDown()
    {
        delete canvas;
    }

    void testEnsureVisibility()
    {
        canvas->resize(100, 100);
        CPPUNIT_ASSERT(canvas->width() == 100);
        CPPUNIT_ASSERT(canvas->height() == 100);

        QCanvasRectangle item(90, 90, 10, 10, canvas);
        canvas->ensureVisibility(&item);
        CPPUNIT_ASSERT(canvas->width() >= 100);
        CPPUNIT_ASSERT(canvas->height() >= 100);

        item.moveBy(100, 200);
        canvas->ensureVisibility(&item);
        CPPUNIT_ASSERT(canvas->width() >= 200);
        CPPUNIT_ASSERT(canvas->height() >= 300);
    }

    void testToGrid()
    {
        int gridSize = Settings::instance()->gridSize();

        Settings::instance()->setGridSize(10);
        CPPUNIT_ASSERT(canvas->toGrid(QPoint(14, 14)) == QPoint(10, 10));

        Settings::instance()->setGridSize(gridSize);
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(GridCanvasTest);

