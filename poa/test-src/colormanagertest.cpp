#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "blockmodel.h"
#include "colormanager.h"
#include "gridcanvas.h"

/** Class to test the ColorManager class.
*/
class ColorManagerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ColorManagerTest);
    CPPUNIT_TEST(testSerialize);
    CPPUNIT_TEST_SUITE_END();

private:
    GridCanvas *canvas;
    ColorManager *colorManager;

public:

    void setUp()
    {
        canvas = new GridCanvas("canvas");
        colorManager = new ColorManager(canvas, Palette::createLightPalette());
    }

    void tearDown()
    {
        delete colorManager;
        delete canvas;
    }

    void testSerialize()
    {
        BlockModel block("Type", "Description");
        block.setClock(100);
        QColor c1 = colorManager->color(&block);

        BlockModel block2("Type", "Description");
        block2.setClock(200);
        QColor c2 = colorManager->color(&block2);

        QDomDocument document;
        QDomElement element = colorManager->serialize(&document);

        ColorManager colorManager2(canvas, Palette::createLightPalette());
        colorManager2.deserialize(element);

        CPPUNIT_ASSERT(c1 == colorManager2.color(&block));
        CPPUNIT_ASSERT(c2 == colorManager2.color(&block2));
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(ColorManagerTest);
