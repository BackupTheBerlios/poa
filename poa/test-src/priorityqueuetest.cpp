#include "genericpriorityqueue.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Exception.h>


class IntegerItem : public PriorityItem
{
public:
    IntegerItem(int i)
    {
	value_ = i;
    }

    bool higherPriority(const PriorityItem *other) const
    {
	return this->value_ < static_cast<const IntegerItem*>(other)->value_;
    }

    void change(int value)
    {
	value_ = value;
	updatePriority();
    }

    QString image() const
    {
	return QString::number(value_);
    }

private:
    int value_;
};

typedef GenericPriorityQueue<IntegerItem> IntegerQueue;

/**
 * Class to test the priority queue
 */
class PriorityQueueTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(PriorityQueueTest);
    CPPUNIT_TEST(testAscending);
    CPPUNIT_TEST(testDescending);
    CPPUNIT_TEST(test2Up1Down);
    CPPUNIT_TEST(test2Up1DownChanging);
    CPPUNIT_TEST(testChanging);
    CPPUNIT_TEST(testRemoving);
    CPPUNIT_TEST_SUITE_END();

private:
    IntegerQueue queue_;

protected:
    void checkIntegrity()
    {
	QString errors = queue_.checkIntegrity();
	if (errors != QString::null) {
	    const char *s = errors;
	    CPPUNIT_ASSERT_MESSAGE(s, errors == QString::null);
	}
    }

public:
    //    void setUp()
    //    {
    //    }

    //    void tearDown()
    //    {
    //    }

    void testAscending()
    {
	IntegerItem *items[10];
	int i;
	for (i = 0; i < 10; ++i) {
	    items[i] = new IntegerItem(i);
	    queue_.insert(items[i]);
	    checkIntegrity();
	}
	CPPUNIT_ASSERT(queue_.size() == 10);

	for (i = 0; i < 10; ++i) {

	    CPPUNIT_ASSERT(queue_.head() == items[i]);
	    CPPUNIT_ASSERT(queue_.removeHead() == items[i]);
	    CPPUNIT_ASSERT(((int) queue_.size()) == 10 - i - 1);
	    checkIntegrity();
	    delete items[i];
	}
    }

    void testDescending()
    {
	IntegerItem *items[10];
	int i;
	for (i = 9; i >= 0; --i) {
	    items[i] = new IntegerItem(i);
	    queue_.insert(items[i]);
	    checkIntegrity();
	}
	CPPUNIT_ASSERT(queue_.size() == 10);

	for (i = 0; i < 10; ++i) {

	    CPPUNIT_ASSERT(queue_.head() == items[i]);
	    CPPUNIT_ASSERT(queue_.removeHead() == items[i]);
	    CPPUNIT_ASSERT(((int) queue_.size()) == 10 - i - 1);
	    checkIntegrity();
	    delete items[i];
	}
    }

    void test2Up1Down()
    {
	IntegerItem *items[10];
	items[0] = new IntegerItem(200);
	items[1] = new IntegerItem(201);
	items[2] = new IntegerItem(50);
	items[3] = new IntegerItem(205);
	items[4] = new IntegerItem(206);
	items[5] = new IntegerItem(52);
	items[6] = new IntegerItem(100);
	items[7] = new IntegerItem(101);
	items[8] = new IntegerItem(51);
	items[9] = new IntegerItem(150);
	
	int i;
	for (i = 0; i < 10; ++i) {
	    checkIntegrity();
	    CPPUNIT_ASSERT(queue_.size() == (unsigned) i);
	    queue_.insert(items[i]);
	}
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[2]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[8]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[5]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[6]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[7]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[9]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[0]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[1]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[3]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[4]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.isEmpty());
	for (i = 0; i < 10; ++i) {
	    delete items[i];
	}
    }

    void test2Up1DownChanging()
    {
	IntegerItem *items[10];
	items[0] = new IntegerItem(200);
	items[1] = new IntegerItem(201);
	items[2] = new IntegerItem(50);
	items[3] = new IntegerItem(205);
	items[4] = new IntegerItem(206);
	items[5] = new IntegerItem(52);
	items[6] = new IntegerItem(100);
	items[7] = new IntegerItem(101);
	items[8] = new IntegerItem(51);
	items[9] = new IntegerItem(150);
	
	int i;
	for (i = 0; i < 10; ++i) {
	    checkIntegrity();
	    CPPUNIT_ASSERT(queue_.size() == (unsigned) i);
	    queue_.insert(items[i]);
	}
	checkIntegrity();
	for (i = 0; i < 10; ++i) {
	    items[i]->change(i);
	    checkIntegrity();
	}
	CPPUNIT_ASSERT(queue_.removeHead() == items[0]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[1]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[2]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[3]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[4]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[5]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[6]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[7]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[8]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[9]);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.isEmpty());
	for (i = 0; i < 10; ++i) {
	    delete items[i];
	}
    }

    void testChanging()
    {
	IntegerItem *items[100];
	int i;
	for (i = 0; i < 100; ++i) {
	    checkIntegrity();
	    CPPUNIT_ASSERT((int) queue_.size() == i);
	    items[i] = new IntegerItem(i);
	    queue_.insert(items[i]);
	}
	checkIntegrity();
	
	for (i = 99; i >= 0; --i) {
            CPPUNIT_ASSERT((int) queue_.size() == 100);
	    items[i]->change(2 * (100 - i));
	    checkIntegrity();
	}

        i = 100;
	while (!queue_.isEmpty()) {
	    CPPUNIT_ASSERT((int) queue_.size() == i);
	    checkIntegrity();
	    --i;
	    CPPUNIT_ASSERT(queue_.removeHead() == items[i]);
	}
	checkIntegrity();
	CPPUNIT_ASSERT(i == 0);

	for (i = 0; i < 100; ++i) {
	    delete items[i];
	}
    }

    void testRemoving()
    {
	IntegerItem *items[100];
	int i;
	for (i = 0; i < 100; i += 2) {
	    items[i] = new IntegerItem(i);
	}
	for (i = 1; i < 100; i += 2) {
	    items[i] = new IntegerItem((i / 2 + 1) * 5);
	}
	for (i = 0; i < 100; ++i) {
	    CPPUNIT_ASSERT(!items[i]->isInQueue());
	    queue_.insert(items[i]);
	    CPPUNIT_ASSERT(items[i]->isInQueue());
	}
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.size() == 100);
	CPPUNIT_ASSERT(queue_.head() == items[0]);

	for (i = 10; i < 20; ++i) {
	    queue_.remove(items[i]);
	    CPPUNIT_ASSERT(!items[i]->isInQueue());
	    checkIntegrity();
	}
	CPPUNIT_ASSERT(queue_.size() == 90);
	CPPUNIT_ASSERT(queue_.head() == items[0]);
	for (i = 30; i < 40; ++i) {
	    queue_.remove(items[i]);
	    CPPUNIT_ASSERT(!items[i]->isInQueue());
	    checkIntegrity();
	}
	CPPUNIT_ASSERT(queue_.size() == 80);
	CPPUNIT_ASSERT(queue_.head() == items[0]);
	for (i = 99; i >= 80; --i) {
	    queue_.remove(items[i]);
	    CPPUNIT_ASSERT(!items[i]->isInQueue());
	    checkIntegrity();
	}
	CPPUNIT_ASSERT(queue_.size() == 60);
	CPPUNIT_ASSERT(queue_.head() == items[0]); // 0
	items[0]->change(17);
	CPPUNIT_ASSERT(queue_.head() == items[2]);

	for (i = 50; i < 60; ++i) {
	    queue_.remove(items[i]);
	    CPPUNIT_ASSERT(!items[i]->isInQueue());
	    checkIntegrity();
	}
	CPPUNIT_ASSERT(queue_.size() == 50);
	CPPUNIT_ASSERT(queue_.head() == items[2]); // 2
	items[2]->change(113);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.head() == items[4]); // 4
	items[4]->change(7);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.head() == items[1]); // 5
	queue_.removeHead();
	CPPUNIT_ASSERT(!items[1]->isInQueue());
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.head() == items[6]); // 6
	items[6]->change(1);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.head() == items[6]); // 1
	items[6]->change(8);
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.removeHead() == items[4]); // 7
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.head() == items[8] || queue_.head() == items[6]);
	queue_.removeHead(); // 8
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.head() == items[8] || queue_.head() == items[6]);
	queue_.removeHead(); // 8
	checkIntegrity();
	items[9]->change(19);
	CPPUNIT_ASSERT(queue_.head() == items[3]); // 10
	CPPUNIT_ASSERT(queue_.size() == 46);

	queue_.clear();
	checkIntegrity();
	CPPUNIT_ASSERT(queue_.size() == 0 && queue_.isEmpty());
       
	for (i = 0; i < 100; ++i) {
	    CPPUNIT_ASSERT(!items[i]->isInQueue());
	    delete items[i];
	}
    }

};

/// \brief You have to register the test suite "ComplexNumberTest". In this
/// it will be recognized by the may test program which drives the
/// different tests in a specific package test directory.
CPPUNIT_TEST_SUITE_REGISTRATION(PriorityQueueTest);
