#include"example.h" 
#include<cppunit/extensions/HelperMacros.h>
#include<cppunit/Exception.h>

/** Class to test the "Complex.h" class 
*
*  Use the HelperMacros.h header file to establish in a easy 
*  way the "ComplexNumberTest" test suite in which you can 
*  place all the tests ("testEquality", "testAddition"... ) 
*  concerning the tested class "Complex.h"
*/
class ComplexNumberTest : public CppUnit::TestFixture
{
/// \brief  Definition of the unit test suite "ComplexNumberTest",
/// don't forget to add all those tests("testEquality",
/// "testAddition"... ) you would like to run. 
 CPPUNIT_TEST_SUITE( ComplexNumberTest );
 CPPUNIT_TEST( testEquality );
 CPPUNIT_TEST( testAddition );
 CPPUNIT_TEST_SUITE_END();
 private:
   Complex *m_10_1, *m_1_1, *m_11_2;
 public:
/// \brief Override setUp() to initialize the 
///        variables you will use for test
   void setUp()
   {
     m_10_1 = new Complex(10,1);
     m_1_1  = new Complex(1,1);
     m_11_2 = new Complex(11,2);  
   }
/// Override tearDown() to release any permanent resources you allocated in setUp()
   void tearDown()
   {
     delete m_10_1;
     delete m_1_1;
     delete m_11_2;
   }
/// Write now your own tests, this is the first one
   void testEquality()
   {
     CPPUNIT_ASSERT( *m_10_1 == *m_10_1 );
     CPPUNIT_ASSERT_MESSAGE( "This test fails intentionally", *m_10_1 == *m_1_1 );
   }
/// Write now your own tests, this is the second one
   void testAddition()
   {
     CPPUNIT_ASSERT( *m_10_1 + *m_1_1 == *m_11_2 );
   }
};
/// \brief You have to register the test suite "ComplexNumberTest". In this 
/// it will be recognized by the may test program which drives the 
/// different tests in a specific package test directory.
CPPUNIT_TEST_SUITE_REGISTRATION(ComplexNumberTest);
