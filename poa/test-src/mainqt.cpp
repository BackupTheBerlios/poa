#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/qt/TestRunner.h>
#include <qapplication.h>

int main( int argc, char **argv)
{
   QApplication app(argc, argv);

   CppUnit::QtUi::TestRunner runner;
   runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
   runner.run(true);

   return 0;
}
