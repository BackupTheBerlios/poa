#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "blockmodel.h"
#include "blockview.h"
#include "pinmodel.h"
#include "pinview.h"

/** Class to test the BlockView class.
*/
class BlockViewTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(BlockViewTest);
    CPPUNIT_TEST(testCanvas);
    CPPUNIT_TEST(testDraggable);
    CPPUNIT_TEST(testModel);
    CPPUNIT_TEST(testPins);
    CPPUNIT_TEST(testSignals);
    CPPUNIT_TEST_SUITE_END();

private:
    PinView *inputPin;
    PinView *outputPin;

    BlockModel *blockModel;
    BlockView *block;

public:

    void setUp()
    {
        blockModel = new BlockModel("Type", "Description");
        block = dynamic_cast<BlockView *>(blockModel->createView(0)[0]);

        blockModel->addPin(new PinModel(blockModel, "Input", 0, 0,
                                        PinModel::INPUT));
        blockModel->addPin(new PinModel(blockModel, "Output", 0, 0,
                                        PinModel::OUTPUT));
    }

    void tearDown()
    {
        delete blockModel;
    }

    void testCanvas()
    {
        CPPUNIT_ASSERT(block->canvas() == 0);
    }

    void testDraggable()
    {
        CPPUNIT_ASSERT(block->isDraggable());
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

    void testSignals()
    {
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(BlockViewTest);
