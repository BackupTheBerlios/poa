#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "cpumodel.h"
#include "pinmodel.h"

/** Class to test the CpuModel class.
*/
class CpuModelTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CpuModelTest);
    CPPUNIT_TEST(testAutoSetter);
    CPPUNIT_TEST(testCpuId);
    CPPUNIT_TEST(testProjectPath);
    CPPUNIT_TEST(testSerialize);
    CPPUNIT_TEST_SUITE_END();

private:
    CpuModel *cpu;
    PinModel *inputPin;

public:

    void setUp()
    {
        cpu = new CpuModel("Type", "Description");

        inputPin = new PinModel(cpu, "Input", 0, 0, PinModel::INPUT);
        cpu->addPin(inputPin);
    }

    void tearDown()
    {
        delete cpu;
        inputPin = 0;
    }

    void testAutoSetter()
    {
        CPPUNIT_ASSERT(cpu->autoExecTime() == true);
        cpu->setAutoExecTime(false);
        CPPUNIT_ASSERT(cpu->autoExecTime() == false);

        CPPUNIT_ASSERT(cpu->autoOffset() == true);
        cpu->setAutoOffset(false);
        CPPUNIT_ASSERT(cpu->autoOffset() == false);
    }

    void testCpuId()
    {
        CPPUNIT_ASSERT(cpu->cpuId() == -1);

        cpu->setCpuId(100);
        CPPUNIT_ASSERT(cpu->cpuId() == 100);
    }

    void testProjectPath()
    {
        CPPUNIT_ASSERT(cpu->projectPath().isNull());

        cpu->setProjectPath("ProjectPath");
        CPPUNIT_ASSERT(cpu->projectPath() == "ProjectPath");
    }

    void testSerialize()
    {
        cpu->setCpuId(1);
        cpu->setAutoExecTime(false);
        cpu->setAutoOffset(false);

        cpu->setSaveSource(false);

        QDomDocument document;
        QDomElement element = cpu->serialize(&document);

        CpuModel cpu2(element);
        CPPUNIT_ASSERT(cpu2.cpuId() == 1);
        CPPUNIT_ASSERT(cpu2.autoExecTime() == false);
        CPPUNIT_ASSERT(cpu2.autoOffset() == false);
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(CpuModelTest);
