#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <iostream>

#include <qapplication.h>
#include <qfileinfo.h>

#include "util.h"

/**  Main class for all the CppUnit test classes
*
*  This will be the driver class of all your CppUnit test classes.
*  - All registered CppUnit test classes will be run.
*  - You can also modify the output (text, compiler, XML).
*  - This class will also integrate CppUnit test with Oval
*/


int main( int argc, char **argv)
{
    // initialize qt
    QApplication app(argc, argv);
    Util::setExecutablePath(QFileInfo(qApp->argv()[0]).dirPath(true));


    // get the top level suite from the registry
    CppUnit::Test *suite
        = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

    // add the test to the list of test to run
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(suite);

    // run the tests
    bool wasSuccessful = runner.run();
    return (wasSuccessful) ? 0 : 1;
}
