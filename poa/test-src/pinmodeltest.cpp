#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "blockmodel.h"
#include "pinmodel.h"

/** Class to test the PinModel class.
*/
class PinModelTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(PinModelTest);
    CPPUNIT_TEST(testAddress);
    CPPUNIT_TEST(testAttach);
    CPPUNIT_TEST(testBits);
    CPPUNIT_TEST(testConnectable);
    CPPUNIT_TEST(testDetach);
    CPPUNIT_TEST(testId);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testPosition);
    CPPUNIT_TEST(testSerialize);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST_SUITE_END();

private:
    PinModel *inputPin;
    PinModel *outputPin;
    PinModel *episodicPin;

public:

    void setUp()
    {
        inputPin = new PinModel(NULL, "Input", 0, 0, PinModel::INPUT);
        outputPin = new PinModel(NULL, "Output", 0, 0, PinModel::OUTPUT);
        episodicPin = new PinModel(NULL, "Episodic", 0, 0, PinModel::EPISODIC);
    }

    void tearDown()
    {
        delete inputPin;
        delete outputPin;
        delete episodicPin;
    }


    void testAddress()
    {
        CPPUNIT_ASSERT(inputPin->address() == 0);

        inputPin->setAddress(1);
        CPPUNIT_ASSERT(inputPin->address() == 1);
    }

    void testAttach()
    {
        CPPUNIT_ASSERT(inputPin->connected() == 0);

        inputPin->attach(outputPin);
        CPPUNIT_ASSERT(inputPin->connected() == outputPin);
    }

    void testBits()
    {
        CPPUNIT_ASSERT(inputPin->bits() == 0);

        inputPin->setBits(1);
        CPPUNIT_ASSERT(inputPin->bits() == 1);
    }

    void testConnectable()
    {
        CPPUNIT_ASSERT(inputPin->isConnectable(outputPin));
        CPPUNIT_ASSERT(inputPin->isConnectable(episodicPin));
        CPPUNIT_ASSERT(!inputPin->isConnectable(inputPin));

        CPPUNIT_ASSERT(outputPin->isConnectable(inputPin));
        CPPUNIT_ASSERT(outputPin->isConnectable(episodicPin));
        CPPUNIT_ASSERT(!outputPin->isConnectable(outputPin));

        CPPUNIT_ASSERT(episodicPin->isConnectable(inputPin));
        CPPUNIT_ASSERT(episodicPin->isConnectable(outputPin));
        CPPUNIT_ASSERT(episodicPin->isConnectable(episodicPin));
    }

    void testDetach()
    {
        inputPin->attach(outputPin);
        inputPin->detach();
        CPPUNIT_ASSERT(inputPin->connected() == 0);
    }

    void testId()
    {
        CPPUNIT_ASSERT(inputPin->id() == 0);

        inputPin->setId(1);
        CPPUNIT_ASSERT(inputPin->id() == 1);
    }

    void testName()
    {
        CPPUNIT_ASSERT(inputPin->name() == "Input");

        inputPin->setName("Name");
        CPPUNIT_ASSERT(inputPin->name() == "Name");
    }

    void testPosition()
    {
        CPPUNIT_ASSERT(inputPin->position() == 0);

        inputPin->setPosition(1);
        CPPUNIT_ASSERT(inputPin->position() == 1);

        inputPin->setPosition(1);
        CPPUNIT_ASSERT(inputPin->position() == 1);
    }

    void testSerialize()
    {
        inputPin->setAddress(1);
        inputPin->setBits(2);
        inputPin->setId(3);
        inputPin->setPosition(4);

        QDomDocument document;
        QDomElement element = inputPin->serialize(&document);

        PinModel inputPin2(0, element);
        equals(inputPin, &inputPin2);
    }

    void testType()
    {
        CPPUNIT_ASSERT(inputPin->type() == PinModel::INPUT);

        inputPin->setType(PinModel::OUTPUT);
        CPPUNIT_ASSERT(inputPin->type() == PinModel::OUTPUT);

        inputPin->setType(PinModel::EPISODIC);
        CPPUNIT_ASSERT(inputPin->type() == PinModel::EPISODIC);

        inputPin->setType(PinModel::INPUT);
        CPPUNIT_ASSERT(inputPin->type() == PinModel::INPUT);
    }

    void equals(PinModel *pin1, PinModel *pin2)
    {
        CPPUNIT_ASSERT(pin1->type() == pin2->type());
        CPPUNIT_ASSERT(pin1->position() == pin2->position());
        CPPUNIT_ASSERT(pin1->bits() == pin2->bits());
        CPPUNIT_ASSERT(pin1->id() == pin2->id());
        CPPUNIT_ASSERT(pin1->name() == pin2->name());
        CPPUNIT_ASSERT(pin1->address() == pin2->address());
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(PinModelTest);
