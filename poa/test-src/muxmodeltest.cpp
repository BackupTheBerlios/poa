#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "muxmodel.h"
#include "pinmodel.h"

/** Class to test the MuxModel class.
*/
class MuxModelTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MuxModelTest);
    CPPUNIT_TEST(testMapping);
    CPPUNIT_TEST_SUITE_END();

private:
    MuxModel *model;
public:
    void setUp()
    {
        model = new MuxModel("Mux", "Description");
    }

    void tearDown()
    {
        delete model;
    }

    void testMapping()
    {
        MuxMapping *mapping = new MuxMapping
            (new PinModel(model, "inpin"), new PinModel(model, "outpin"),
             0, 8, 0, 8);
        model->addMuxMapping(mapping);
        CPPUNIT_ASSERT(model->mappings()->count() == 1);

        model->removeMuxMapping(mapping);
        CPPUNIT_ASSERT(model->mappings()->isEmpty());
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(MuxModelTest);
