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
    CPPUNIT_TEST(testColor);
    CPPUNIT_TEST(testColors);
    CPPUNIT_TEST(testDeleteBlock);
    CPPUNIT_TEST(testDragBy);
    CPPUNIT_TEST(testItem);
    CPPUNIT_TEST(testLightPalette);
    CPPUNIT_TEST(testPalette);
    CPPUNIT_TEST(testRtti);
    CPPUNIT_TEST(testSerialize);
    CPPUNIT_TEST(testStrongPalette);
    CPPUNIT_TEST(testUpdateBlock);
    CPPUNIT_TEST_SUITE_END();

private:
    GridCanvas *canvas;
    ColorManager *colorManager;
    Palette *palette;

public:

    void setUp()
    {
        canvas = new GridCanvas("canvas");
        palette = new Palette("Mock Palette");
        palette->addColor(QColor("#00FF00"));
        palette->addColor(QColor("#0000FF"));
        colorManager = new ColorManager(canvas, palette);
    }

    void tearDown()
    {
        delete colorManager;
        delete canvas;
    }

    void testColor()
    {
        BlockModel block("", "");
        block.setClock(100);

        CPPUNIT_ASSERT(colorManager->color(&block) == palette->color(0));
        CPPUNIT_ASSERT(colorManager->color(&block) == palette->color(0));

        BlockModel block2("Type", "Description");
        block2.setClock(200);
        BlockModel block3("Type", "Description");
        block3.setClock(300);

        CPPUNIT_ASSERT(colorManager->color(&block2) == palette->color(1));
        CPPUNIT_ASSERT(colorManager->color(&block3) == palette->color(0));
    }

    void testColors()
    {
        BlockModel block("", "");
        CPPUNIT_ASSERT(colorManager->color(0)
                       == Settings::instance()->defaultBrushColor());
        CPPUNIT_ASSERT(colorManager->color(&block)
                       == Settings::instance()->defaultBrushColor());
        CPPUNIT_ASSERT(colorManager->activatedColor(&block)
                       == Settings::instance()->activatedColor());
        CPPUNIT_ASSERT(colorManager->selectedColor(&block)
                       == Settings::instance()->selectedColor());
    }

    void testDeleteBlock()
    {
        BlockModel *block = new BlockModel("", "");
        block->setClock(100);
        CPPUNIT_ASSERT(colorManager->color(block) == palette->color(0));

        delete block;

        block = new BlockModel("", "");
        block->setClock(100);
        CPPUNIT_ASSERT(colorManager->color(block) == palette->color(1));

        delete block;
    }

    void testDragBy()
    {
        CPPUNIT_ASSERT(colorManager->x() == 0);
        CPPUNIT_ASSERT(colorManager->y() == 0);

        QPoint dragged = colorManager->dragBy(10, 15);
        CPPUNIT_ASSERT(colorManager->x() == 10);
        CPPUNIT_ASSERT(colorManager->y() == 15);
        CPPUNIT_ASSERT(dragged.x() == 10);
        CPPUNIT_ASSERT(dragged.y() == 15);
    }

    void testItem()
    {
        CPPUNIT_ASSERT(colorManager->item() == colorManager);
    }

    void testLightPalette()
    {
        testPalette(Palette::createLightPalette());
    }

    void testStrongPalette()
    {
        testPalette(Palette::createStrongPalette());
    }

    void testPalette()
    {
        CPPUNIT_ASSERT(palette->size() == 2);
    }

    void testPalette(Palette *palette)
    {
        CPPUNIT_ASSERT(palette->size() > 0);
        delete palette;
    }

    void testRtti()
    {
        CPPUNIT_ASSERT(colorManager->rtti() == ColorManager::RTTI);
    }

    void testSerialize()
    {
        BlockModel block("Type", "Description");
        block.setClock(100);
        QColor c1 = colorManager->color(&block);

        BlockModel block2("Type", "Description");
        block2.setClock(200);
        QColor c2 = colorManager->color(&block2);

        BlockModel block3("Type", "Description");
        QColor c3 = colorManager->color(&block3);

        QDomDocument document;
        QDomElement element = colorManager->serialize(&document);

        Palette *palette2 = new Palette("Mock Palette");
        palette2->addColor(QColor("#00FF00"));
        palette2->addColor(QColor("#0000FF"));

        ColorManager colorManager2(canvas, palette2);
        colorManager2.deserialize(element);

        CPPUNIT_ASSERT(c1 == colorManager2.color(&block));
        CPPUNIT_ASSERT(c2 == colorManager2.color(&block2));
        CPPUNIT_ASSERT(c3 == colorManager2.color(&block3));
    }

    void testUpdateBlock()
    {
        BlockModel *block = new BlockModel("", "");
        block->setClock(100);
        CPPUNIT_ASSERT(colorManager->color(block) == palette->color(0));

        block->setClock(200);
        block->updatePerformed();
        CPPUNIT_ASSERT(colorManager->color(block) == palette->color(1));

        delete block;
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(ColorManagerTest);
