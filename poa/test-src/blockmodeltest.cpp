#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "blockmodel.h"
#include "pinmodel.h"

/** Class to test the BlockModel class.
*/
class BlockModelTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(BlockModelTest);
    CPPUNIT_TEST(testAddPin);
    CPPUNIT_TEST(testClock);
    CPPUNIT_TEST(testDeletePin);
    CPPUNIT_TEST(testDescription);
    CPPUNIT_TEST(testExecTime);
    CPPUNIT_TEST(testFindPinById);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testOffset);
    CPPUNIT_TEST(testPinSetter);
    CPPUNIT_TEST(testSerialize);
    CPPUNIT_TEST(testType);
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

    void testAddPin()
    {
        PinModel *inputPin2
            = new PinModel(block, "Input", 0, 0, PinModel::INPUT);
        block->addPin(inputPin2);

        CPPUNIT_ASSERT(inputPin->id() != inputPin2->id());
        CPPUNIT_ASSERT(block->pins().count() == 2);

        block->deletePin(inputPin2);
        CPPUNIT_ASSERT(block->pins().count() == 1);
    }

    void testClock()
    {
        CPPUNIT_ASSERT(block->clock() == 0);

        block->setClock(100);
        CPPUNIT_ASSERT(block->clock() == 100);
    }

    void testDeletePin()
    {
        CPPUNIT_ASSERT(block->pins().count() == 1);

        block->deletePin(inputPin);
        CPPUNIT_ASSERT(block->pins().isEmpty());
    }

    void testDescription()
    {
        CPPUNIT_ASSERT(block->description() == "Description");

        block->setDescription("MyDescription");
        CPPUNIT_ASSERT(block->description() == "MyDescription");
    }

    void testExecTime()
    {
        CPPUNIT_ASSERT(block->execTime() == 0);

        block->setExecTime(100);
        CPPUNIT_ASSERT(block->execTime() == 100);
    }

    void testFindPinById()
    {
        int id = inputPin->id();
        CPPUNIT_ASSERT(block->findPinById(id) == inputPin);

        block->deletePin(inputPin);
        CPPUNIT_ASSERT(block->findPinById(id) == 0);
    }

    void testId()
    {
        CPPUNIT_ASSERT(block->id() == 0);

        block->setId(100);
        CPPUNIT_ASSERT(block->id() == 100);
    }

    void testName()
    {
        CPPUNIT_ASSERT(block->name() == "Type");

        block->setName("Name");
        CPPUNIT_ASSERT(block->name() == "Name");
    }

    void testOffset()
    {
        CPPUNIT_ASSERT(block->offset() == 0);

        block->setOffset(100);
        CPPUNIT_ASSERT(block->offset() == 100);
    }

    void testPinSetter()
    {
        CPPUNIT_ASSERT(block->hasEpisodicPins() == true);
        block->setHasEpisodicPins(false);
        CPPUNIT_ASSERT(block->hasEpisodicPins() == false);

        CPPUNIT_ASSERT(block->hasInputPins() == true);
        block->setHasInputPins(false);
        CPPUNIT_ASSERT(block->hasInputPins() == false);

        CPPUNIT_ASSERT(block->hasOutputPins() == true);
        block->setHasOutputPins(false);
        CPPUNIT_ASSERT(block->hasOutputPins() == false);

        CPPUNIT_ASSERT(block->hasRuntime() == true);
        block->setHasRuntime(false);
        CPPUNIT_ASSERT(block->hasRuntime() == false);
    }

    void testSerialize()
    {
        block->setClock(1);
        block->setOffset(2);
        block->setExecTime(3);
        block->setHasEpisodicPins(false);
        block->setHasOutputPins(false);
        block->setName("MyName");
        block->setDescription("MyDescription");
        block->setType("MyType");
        block->addPin(new PinModel(block, "Output", 0, 0, PinModel::OUTPUT));
        block->addPin(new PinModel(block, "Episod", 0, 0, PinModel::EPISODIC));

        QDomDocument document;
        QDomElement element = block->serialize(&document);

        BlockModel block2(element);
        CPPUNIT_ASSERT(block2.clock() == 1);
        CPPUNIT_ASSERT(block2.offset() == 2);
        CPPUNIT_ASSERT(block2.execTime() == 3);
        CPPUNIT_ASSERT(block2.name() == "MyName");
        CPPUNIT_ASSERT(block2.description() == "MyDescription");
        CPPUNIT_ASSERT(block2.type() == "MyType");
        CPPUNIT_ASSERT(block2.hasInputPins() == true);
        CPPUNIT_ASSERT(block2.hasRuntime() == true);
        CPPUNIT_ASSERT(block2.hasOutputPins() == false);
        CPPUNIT_ASSERT(block2.hasEpisodicPins() == false);

        CPPUNIT_ASSERT(block2.pins().count() == 3);
    }

    void testType()
    {
        CPPUNIT_ASSERT(block->type() == "Type");

        block->setType("MyType");
        CPPUNIT_ASSERT(block->type() == "MyType");
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(BlockModelTest);
