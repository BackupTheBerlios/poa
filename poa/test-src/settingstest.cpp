#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "settings.h"

#include <qcolor.h>
#include <qfont.h>

/** Class to test the Settings class.
*/
class SettingsTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(SettingsTest);
    CPPUNIT_TEST(testBool);
    CPPUNIT_TEST(testColor);
    CPPUNIT_TEST(testFont);
    CPPUNIT_TEST(testNum);
    CPPUNIT_TEST(testString);
    CPPUNIT_TEST_SUITE_END();

private:
    Settings *settings;

public:

    void setUp()
    {
        settings = new Settings("/POA/POATest");
    }

    void tearDown()
    {
        delete settings;

        QSettings qs;
        QStringList keys = qs.entryList("/POA/POATest");
        QStringList::iterator it = keys.begin();
        for (; it != keys.end(); ++it) {
            qs.removeEntry(*it);
        }
    }

    void testBool()
    {
        settings->set("key", true);
        CPPUNIT_ASSERT(settings->getBool("key") == true);

        settings->set("key", false);
        CPPUNIT_ASSERT(settings->getBool("key") == false);
    }

    void testColor()
    {
        CPPUNIT_ASSERT(settings->defaultBrushColor() == Qt::white);
        CPPUNIT_ASSERT(settings->defaultColor() == Qt::black);
    }

    void testFont()
    {
        QFont font;

        settings->set("key", font.toString());
        CPPUNIT_ASSERT(settings->getFont("key") == font);
    }

    void testNum()
    {
        settings->set("numkey", 1);
        CPPUNIT_ASSERT(settings->getNum("numkey") == 1);

        settings->set("numkey", 0);
        CPPUNIT_ASSERT(settings->getNum("numkey") == 0);

        settings->set("numkey", -1);
        CPPUNIT_ASSERT(settings->getNum("numkey") == -1);
    }


    void testString()
    {
        settings->set("key", QString("value"));
        CPPUNIT_ASSERT(settings->get("key") == "value");

        settings->set("key", "value");
        CPPUNIT_ASSERT(settings->get("key") == "value");

        settings->set("key", "");
        CPPUNIT_ASSERT(settings->get("key") == "");
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(SettingsTest);
