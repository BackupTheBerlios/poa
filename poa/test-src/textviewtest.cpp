#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "gridcanvas.h"
#include "textview.h"

/** Class to test the TextView class.
*/
class TextViewTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TextViewTest);
    CPPUNIT_TEST(testCanvas);
    CPPUNIT_TEST(testSerialize);
    CPPUNIT_TEST(testText);
    CPPUNIT_TEST_SUITE_END();

private:
    TextView *textView;
    GridCanvas *canvas;

public:

    void setUp()
    {
        canvas = new GridCanvas("canvas");
        textView = new TextView("mytext", canvas);
    }

    void tearDown()
    {
        delete textView;
        delete canvas;
    }

    void testCanvas()
    {
        CPPUNIT_ASSERT(textView->canvas() == canvas);
    }

    void testSerialize()
    {
    }

    void testText()
    {
        CPPUNIT_ASSERT(textView->text() == "mytext");
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(TextViewTest);
