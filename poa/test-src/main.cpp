#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/Test.h>
#include <cppunit/TestListener.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/TestResult.h>
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

class CopyListener : public CppUnit::TestListener
{
public:

    void startSuite(CppUnit::Test *suite)
    {
        if (suite->getName().find("All Tests") == std::string::npos) {
            std::cout << suite->getName() << " ";
            std::cout.flush();
        }
    }

    void endSuite(CppUnit::Test *suite)
    {
        if (suite->getName().find("All Tests") == std::string::npos) {
//              std::cout << " Done" << std::endl;
            std::cout << std::endl;
            std::cout.flush();
        }

//          std::string name = suite->getName();
//          std::string filename = name.substr(0, name.length() - 4).c_str();
//          std::transform(filename.begin(), filename.end(), filename.begin(),
//                         tolower);
//          filename = "../src/" + filename + ".da";
//          std::cout << filename << std::endl;

//          std::string command = "ls -al " + filename;
//          system(command.c_str());
    }

};

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

    // initialize copy listener
    CopyListener copyListener;
    runner.eventManager().addListener(&copyListener);

    // run the tests
    bool wasSuccessful = runner.run();
    return (wasSuccessful) ? 0 : 1;
}
