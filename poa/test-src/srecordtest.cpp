#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "poaexception.h"
#include "srecord.h"

/** Class to test the SRecord class.
*/
class SRecordTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SRecordTest);
    CPPUNIT_TEST(testAddress);
    CPPUNIT_TEST(testChecksum);
    CPPUNIT_TEST(testData);
    CPPUNIT_TEST(testDataSize);
    CPPUNIT_TEST_EXCEPTION(testInvalidAddress, PoaException);
    CPPUNIT_TEST_EXCEPTION(testInvalidChecksum, PoaException);
    CPPUNIT_TEST_EXCEPTION(testInvalidCount, PoaException);
    CPPUNIT_TEST_EXCEPTION(testInvalidCountTooLow, PoaException);
    CPPUNIT_TEST_EXCEPTION(testInvalidCountTooHigh, PoaException);
    CPPUNIT_TEST_EXCEPTION(testInvalidData, PoaException);
    CPPUNIT_TEST_EXCEPTION(testInvalidLength, PoaException);
    CPPUNIT_TEST_EXCEPTION(testInvalidType, PoaException);
    CPPUNIT_TEST(testMinLength);
    CPPUNIT_TEST(testType);
    CPPUNIT_TEST_SUITE_END();

private:
    SRecord *record1;
    SRecord *record2;
    SRecord *record3;
    SRecord *record4;

public:

    void setUp()
    {
        record1
            = new SRecord("S1130000285F245F2212226A000424290008237C2A", 42);
        record2 = new SRecord("S105ABCDAABB01", 14);
        record3 = new SRecord("S103010110", 10);
        record4 = new SRecord("S1040000AA00zzzzzzzzzzzz\n", 25);
    }

    void tearDown()
    {
        delete record1;
        delete record2;
        delete record3;
        delete record4;
    }

    void testAddress()
    {
        CPPUNIT_ASSERT(record1->address() == 0x0);
        CPPUNIT_ASSERT(record2->address() == 0xABCD);
        CPPUNIT_ASSERT(record3->address() == 0x0101);
        CPPUNIT_ASSERT(record4->address() == 0x0000);
    }

    void testChecksum()
    {
        CPPUNIT_ASSERT(record1->checksum() == 0x2A);
        CPPUNIT_ASSERT(record2->checksum() == 0x01);
        CPPUNIT_ASSERT(record3->checksum() == 0x10);
        CPPUNIT_ASSERT(record4->checksum() == 0x00);
    }

    void testData()
    {
        CPPUNIT_ASSERT(record2->data()[0] == 0xAA);
        CPPUNIT_ASSERT(record2->data()[1] == 0xBB);

        CPPUNIT_ASSERT(record4->data()[0] == 0xAA);
    }

    void testDataSize()
    {
        CPPUNIT_ASSERT(record1->dataSize() == 0x10);
        CPPUNIT_ASSERT(record2->dataSize() == 0x2);
        CPPUNIT_ASSERT(record3->dataSize() == 0x0);
        CPPUNIT_ASSERT(record4->dataSize() == 0x1);
    }

    void testInvalidAddress()
    {
        SRecord("S103ZZZZ00", 10);
    }

    void testInvalidChecksum()
    {
        SRecord("S1030000ZZ", 10);
    }

    void testInvalidCount()
    {
        SRecord("S1ZZ000000", 10);
    }

    void testInvalidCountTooLow()
    {
        SRecord("S102000000", 10);
    }

    void testInvalidCountTooHigh()
    {
        SRecord("S104000000", 10);
    }

    void testInvalidData()
    {
        SRecord("S1040000ZZ00", 12);
    }

    void testInvalidLength()
    {
        SRecord("S1", 2);
    }

    void testInvalidType()
    {
        SRecord("T100000000", 10);
    }

    void testMinLength()
    {
        CPPUNIT_ASSERT(SRecord::minLength() == 10);
    }

    void testType()
    {
        CPPUNIT_ASSERT(record1->type() == SRecord::S1);
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(SRecordTest);
