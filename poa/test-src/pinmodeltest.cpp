#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "pinmodel.h"

/** Class to test the pinmodel class
*/
class PinModelTest : public CppUnit::TestFixture
{
/// \brief  Definition of the unit test suite "PinModelTest",
/// don't forget to add all those tests("testEquality",
/// "testAddition"... ) you would like to run.
        CPPUNIT_TEST_SUITE(PinModelTest);
        CPPUNIT_TEST(testName);
        CPPUNIT_TEST(testID);
        CPPUNIT_TEST_SUITE_END();

private:
        PinModel *inputPin;
        PinModel *outputPin;
        PinModel *episodicPin;
public:
/// \brief Override setUp() to initialize the
///        variables you will use for test
        void setUp()
        {
                inputPin = new PinModel(NULL, "Input", 0, 0, PinModel::INPUT);
                outputPin = new PinModel(NULL, "Output", 0, 0, PinModel::OUTPUT);
                episodicPin = new PinModel(NULL, "Episodic", 0, 0, PinModel::EPISODIC);
        }

/// Override tearDown() to release any permanent resources you allocated in setUp()
        void tearDown()
        {
                delete inputPin;
                delete outputPin;
                delete episodicPin;
        }

/// Write now your own tests, this is the first one
        void testName()
        {
                CPPUNIT_ASSERT(inputPin->name() == "Input");
                CPPUNIT_ASSERT(inputPin->name() != "");
//     CPPUNIT_ASSERT_MESSAGE( "This test fails intentionally", *m_10_1 == *m_1_1 );
        }

        void testID()
        {
                inputPin->setId(1234);
                CPPUNIT_ASSERT(inputPin->id() == 1234);
                CPPUNIT_ASSERT(inputPin->id() != 123);
        }

};
/// \brief You have to register the test suite "ComplexNumberTest". In this
/// it will be recognized by the may test program which drives the
/// different tests in a specific package test directory.
CPPUNIT_TEST_SUITE_REGISTRATION(PinModelTest);
