#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "connectorviewlist.h"
#include "blockmodel.h"
#include "blockview.h"
#include "gridcanvas.h"
#include "pinmodel.h"
#include "pinview.h"

/** Class to test the ConnectorViewList class.
*/
class ConnectorViewListTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ConnectorViewListTest);
    CPPUNIT_TEST(testCanvas);
    CPPUNIT_TEST(testPins);
    CPPUNIT_TEST(testSegments);
    CPPUNIT_TEST(testSignals);
    CPPUNIT_TEST(testVisible);
    CPPUNIT_TEST_SUITE_END();

private:
    PinView *sourcePin;
    PinView *targetPin;
    BlockView *blockView;
    ConnectorViewList *connector;
    GridCanvas *canvas;

public:

    void setUp()
    {
        canvas = new GridCanvas("canvas");
        BlockModel *block = new BlockModel("Type", "Description");
        PinModel *inputPin
            = new PinModel(block, "Input", 0, 0, PinModel::INPUT);
        PinModel *outputPin
            = new PinModel(block, "Output", 0, 0, PinModel::OUTPUT);
        inputPin->attach(outputPin);

        blockView = new BlockView(block, canvas);
        sourcePin = new PinView(inputPin, blockView, PinView::PIN_RIGHT);
        targetPin = new PinView(outputPin, blockView, PinView::PIN_LEFT);

        connector = new ConnectorViewList(sourcePin, targetPin, canvas, 0);
    }

    void tearDown()
    {
        delete blockView->model();
        delete canvas;
        delete connector;
    }

    void testCanvas()
    {
        CPPUNIT_ASSERT(connector->canvas() == canvas);
    }

    void testPins()
    {
        CPPUNIT_ASSERT(connector->source() == sourcePin);
        CPPUNIT_ASSERT(connector->target() == targetPin);
    }

    void testSegments()
    {
        CPPUNIT_ASSERT(connector->allSegments().count() == 0);
    }

    void testSignals()
    {
    }

    void testVisible()
    {
        // test case makes no sense as long as ConnectorViewList is
        // not derived from QCanvasItem

//          CPPUNIT_ASSERT(!connector->visible());

//          connector->show();
//          CPPUNIT_ASSERT(!connector->visible());

        connector->setVisible(true);
//          CPPUNIT_ASSERT(!connector->visible());
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(ConnectorViewListTest);
