#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "blockmodel.h"
#include "blockview.h"
#include "pinmodel.h"
#include "pinview.h"

/** Class to test the PinView class.
*/
class PinViewTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(PinViewTest);
    CPPUNIT_TEST(testCanvas);
    CPPUNIT_TEST(testConnector);
    CPPUNIT_TEST(testModel);
    CPPUNIT_TEST(testSignals);
    CPPUNIT_TEST_SUITE_END();

private:
    PinModel *pinModel;
    PinView *bottomPin;
    PinView *leftPin;
    PinView *rightPin;
    PinView *topPin;
    BlockView *blockView;

public:

    void setUp()
    {
        BlockModel *block = new BlockModel("Type", "Description");
        pinModel = new PinModel(block, "Input", 0, 0, PinModel::INPUT);

        blockView = new BlockView(block, 0);

        bottomPin = new PinView(pinModel, blockView, PinView::PIN_BOTTOM);
        leftPin = new PinView(pinModel, blockView, PinView::PIN_LEFT);
        rightPin = new PinView(pinModel, blockView, PinView::PIN_RIGHT);
        topPin = new PinView(pinModel, blockView, PinView::PIN_TOP);
    }

    void tearDown()
    {
        delete blockView->model();
    }

    void testCanvas()
    {
        CPPUNIT_ASSERT(leftPin->canvas() == 0);
    }

    void testConnector()
    {
        CPPUNIT_ASSERT(leftPin->connector() == 0);
        CPPUNIT_ASSERT(!leftPin->isConnected());
    }

    void testModel()
    {
        CPPUNIT_ASSERT(leftPin->model() == pinModel);
    }

    void testSignals()
    {
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(PinViewTest);
