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
    CPPUNIT_TEST(testDragBy);
    CPPUNIT_TEST(testItem);
    CPPUNIT_TEST(testRemove);
    CPPUNIT_TEST(testRtti);
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
        delete canvas;
    }

    void testCanvas()
    {
        CPPUNIT_ASSERT(textView->canvas() == canvas);
    }

    void testDragBy()
    {
        CPPUNIT_ASSERT(textView->x() == 0);
        CPPUNIT_ASSERT(textView->y() == 0);

        QPoint dragged = textView->dragBy(10, 15);
        CPPUNIT_ASSERT(textView->x() == 10);
        CPPUNIT_ASSERT(textView->y() == 15);
        CPPUNIT_ASSERT(dragged.x() == 10);
        CPPUNIT_ASSERT(dragged.y() == 15);
    }

    void testItem()
    {
        CPPUNIT_ASSERT(textView->item() == textView);
    }

    void testRemove()
    {
        // count is oddly set to 2, what is the other item on the canvas?
        unsigned int count = canvas->allItems().count();
        textView->remove(0);
        CPPUNIT_ASSERT(canvas->allItems().count() == count - 1);
    }

    void testRtti()
    {
        CPPUNIT_ASSERT(textView->rtti() == TextView::RTTI);
    }

    void testSerialize()
    {
        QDomDocument document;
        QDomElement element = textView->serialize(&document);

        TextView textView2(element, canvas);
        CPPUNIT_ASSERT(textView2.text() == "mytext");
    }

    void testText()
    {
        CPPUNIT_ASSERT(textView->text() == "mytext");
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(TextViewTest);
