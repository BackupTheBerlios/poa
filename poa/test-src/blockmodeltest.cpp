#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "blockmodel.h"
#include "pinmodel.h"

/** Class to test the BlockModel class.
*/
class BlockModelTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(BlockModelTest);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST_SUITE_END();

private:
    BlockModel *block;
    PinModel *inputPin;

public:

    void setUp()
    {
        block = new BlockModel("Type", "Description");

        inputPin = new PinModel(block, "Input", 0, 0, PinModel::INPUT);
        block->addPin(inputPin);
    }

    void tearDown()
    {
        delete block;
        inputPin = 0;
    }

    void testName()
    {
        CPPUNIT_ASSERT(block->name() == "Description");
    }

    void testFindPinById()
    {
        CPPUNIT_ASSERT(block->findPinById(inputPin->id()) == inputPin);
    }


};
CPPUNIT_TEST_SUITE_REGISTRATION(BlockModelTest);
