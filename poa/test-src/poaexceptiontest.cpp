#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "poaexception.h"

/** Class to test the PoaException class.
*/
class PoaExceptionTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(PoaExceptionTest);
    CPPUNIT_TEST(testMessage);
    CPPUNIT_TEST_SUITE_END();

private:

public:

    void setUp()
    {
    }

    void tearDown()
    {
    }

    void testMessage()
    {
        PoaException e("Message");
        CPPUNIT_ASSERT(e.message() == "Message");
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(PoaExceptionTest);
