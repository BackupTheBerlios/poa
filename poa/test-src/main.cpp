#include <cppunit/extensions/TestFactoryRegistry.h> 
#include <cppunit/ui/text/TestRunner.h> 
#include <cppunit/CompilerOutputter.h> 
#include <cppunit/TextOutputter.h> 
#include <cppunit/XmlOutputter.h> 
#include <iostream>

/**  Main class for all the CppUnit test classes  
*
*  This will be the driver class of all your CppUnit test classes.
*  - All registered CppUnit test classes will be run. 
*  - You can also modify the output (text, compiler, XML). 
*  - This class will also integrate CppUnit test with Oval
*/


int main( int argc, char **argv)
 {
   /// Get the top level suite from the registry
   CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

   /// Adds the test to the list of test to run
   CppUnit::TextUi::TestRunner runner;
   runner.addTest( suite );

   // Change the default outputter to a compiler error format outputter 
   // uncomment the following line if you need a compiler outputter.
   //runner.setOutputter(new CppUnit::CompilerOutputter( &runner.result(),
   //                                                     std::cerr ) );

   // Change the default outputter to a xml error format outputter 
   // uncomment the following line if you need a xml outputter.
   //runner.setOutputter( new CppUnit::XmlOutputter( &runner.result(),
   //                                                    std::cerr ) );

   /// Run the tests.
       bool wasSuccessful = runner.run();
   // If you want to avoid the CppUnit typical output change the line above 
   // by the following one: 
   //  bool wasSucessful = runner.run("",false,false,false);

   // Return error code 1 if the one of test failed.
   // Uncomment the next line if you want to integrate CppUnit with Oval
   //  std::cout <<"[OVAL] Cppunit-result ="<<!wasSuccessful<<"\n" ;
     return 0;
 }
