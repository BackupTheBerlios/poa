#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>

#include "util.h"

#include <qdir.h>
#include <qfile.h>

/** Class to test the Util class.
*/
class UtilTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(UtilTest);
    CPPUNIT_TEST(testCopyFile);
    CPPUNIT_TEST(testFindResource);
    CPPUNIT_TEST(testSqueeze);
    CPPUNIT_TEST_SUITE_END();

private:

public:

    void setUp()
    {
    }

    void tearDown()
    {
    }

    void testCopyFile()
    {
        QFile source("utiltest.cpp");
        QFile target("utiltest.cpp.tmp");

        CPPUNIT_ASSERT(Util::copyFile(&source, &target));
        CPPUNIT_ASSERT(target.exists());
        CPPUNIT_ASSERT(source.size() == target.size());

        target.remove();
    }

    void testCopyNotExistantFile()
    {
        QFile source("utiltest.cpp.doesnotexist");
        QFile target("utiltest.cpp.tmp.doesnotexist");

        CPPUNIT_ASSERT(!Util::copyFile(&source, &target));
        CPPUNIT_ASSERT(!target.exists());
    }

    void testFindIcon()
    {
        CPPUNIT_ASSERT(Util::findResource("icon.doesnotexist").isNull());
    }

    void testRemoveDir()
    {
        QDir currentDir;
        currentDir.mkdir("utiltest.cpp.subdir");
        currentDir.mkdir("utiltest.cpp.subdir/dir");

        QFile file("utiltest.cpp.subdir/file");
        file.open(IO_WriteOnly);
        file.close();

        QDir dir("utiltest.cpp.subdir");

        CPPUNIT_ASSERT(Util::removeDir(&dir));
        CPPUNIT_ASSERT(!dir.exists());
    }

    void testFindResource()
    {
        QString path = Util::findResource("utiltest.cpp");
        CPPUNIT_ASSERT(path == QFileInfo("utiltest.cpp").absFilePath());
    }

    void testSqueeze()
    {
        CPPUNIT_ASSERT(Util::squeeze("abc", 0, QFont()) == "a...");
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION(UtilTest);
