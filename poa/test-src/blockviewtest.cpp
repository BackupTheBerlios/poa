#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "blockmodel.h"
#include "blockview.h"
#include "gridcanvas.h"
#include "pinmodel.h"
#include "pinview.h"

/** Class to test the BlockView class.
*/
class BlockViewTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(BlockViewTest);
    CPPUNIT_TEST(testCanvas);
    CPPUNIT_TEST(testModel);
    CPPUNIT_TEST(testPins);
    CPPUNIT_TEST(testRtti);
    CPPUNIT_TEST(testSignals);
    CPPUNIT_TEST_SUITE_END();

private:
    PinView *inputPin;
    PinView *outputPin;

    BlockModel *blockModel;
    BlockView *block;
    GridCanvas *canvas;

public:

    void setUp()
    {
        canvas = new GridCanvas("canvas");
        blockModel = new BlockModel("Type", "Description");
        block = dynamic_cast<BlockView *>(blockModel->createView(canvas)[0]);

        blockModel->addPin(new PinModel(blockModel, "Input", 0, 0,
                                        PinModel::INPUT));
        blockModel->addPin(new PinModel(blockModel, "Output", 0, 0,
                                        PinModel::OUTPUT));
    }

    void tearDown()
    {
        delete blockModel;
        //delete canvas;
    }

    void testCanvas()
    {
        CPPUNIT_ASSERT(block->canvas() == canvas);
    }

    void testModel()
    {
        CPPUNIT_ASSERT(block->model() == blockModel);
    }

    void testPins()
    {
        QValueList<PinView*> list = block->pins();
        CPPUNIT_ASSERT(list.count() == 2);
    }

    void testRtti()
    {
        CPPUNIT_ASSERT(block->rtti() == BlockView::RTTI);
    }

    void testSignals()
    {
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(BlockViewTest);
