#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "downloadmanager.h"
#include "poaexception.h"

/** Class to test the DownloadManager class.
*/
class DownloadManagerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(DownloadManagerTest);
    CPPUNIT_TEST(testDownload);
    CPPUNIT_TEST(testFilesize);
    CPPUNIT_TEST_EXCEPTION(testInvalidFile, PoaException);
    CPPUNIT_TEST(testRun);
    CPPUNIT_TEST_SUITE_END();

private:
    DownloadManager *dm;

public:

    void setUp()
    {
        dm = new DownloadManager("resource/srec/simple.srec");
    }

    void tearDown()
    {
        delete dm;
    }

    void testDownload()
    {
        QString filename = "downloadmanagertest.cpp.tmp";
        QFile file(filename);
        if (file.exists()) {
            file.remove();
        }

        CPPUNIT_ASSERT(dm->download(filename));

        CPPUNIT_ASSERT(file.exists());
        CPPUNIT_ASSERT(file.size() == 5 + 2); // 5 bytes header, 2 bytes data

        file.open(IO_ReadOnly);

        CPPUNIT_ASSERT(file.getch() == 0xFF);
        CPPUNIT_ASSERT(file.getch() == 0x01);
        CPPUNIT_ASSERT(file.getch() << 8 | file.getch() == 0x02);
        CPPUNIT_ASSERT(file.getch() == 0x01);
        CPPUNIT_ASSERT(file.getch() == 0xAA);
        CPPUNIT_ASSERT(file.getch() == 0xBB);

        file.close();
        file.remove();
    }

    void testFilesize()
    {
        CPPUNIT_ASSERT(dm->filesize() == 2);
    }

    void testInvalidFile()
    {
        DownloadManager("downloadmanagertest.cpp.doesnotexist");
    }

    void testRun()
    {
        QString filename = "downloadmanagertest.cpp.tmp";
        QFile file(filename);
        if (file.exists()) {
            file.remove();
        }

        CPPUNIT_ASSERT(DownloadManager::run(filename));

        CPPUNIT_ASSERT(file.exists());
        CPPUNIT_ASSERT(file.size() == 5); // 5 bytes header

        file.open(IO_ReadOnly);

        CPPUNIT_ASSERT(file.getch() == 0xFF);
        CPPUNIT_ASSERT(file.getch() == 0x01);
        CPPUNIT_ASSERT(file.getch() << 8 | file.getch() == 0x00);
        CPPUNIT_ASSERT(file.getch() == 0x05);

        file.close();
        file.remove();
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(DownloadManagerTest);
