#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "pinmodel.h"

/** Class to test the PinModel class.
*/
class PinModelTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(PinModelTest);
    CPPUNIT_TEST(testName);
    CPPUNIT_TEST(testID);
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

    void testName()
    {
        CPPUNIT_ASSERT(inputPin->name() == "Input");
        CPPUNIT_ASSERT(inputPin->name() != "");
    }

    void testID()
    {
        inputPin->setId(1234);
        CPPUNIT_ASSERT(inputPin->id() == 1234);
        CPPUNIT_ASSERT(inputPin->id() != 123);
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(PinModelTest);
