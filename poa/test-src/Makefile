CFLAGS   = -pipe -Wall -W -g -D_REENTRANT  -DQT_THREAD_SUPPORT
CXXFLAGS = -pipe -Wall -W -g -D_REENTRANT  -DQT_THREAD_SUPPORT
CPPUNIT_LDFLAGS = $(shell cppunit-config --libs || echo -L../cppunit/lib -lcppunit)
LDFLAGS  = $(CPPUNIT_LDFLAGS) -L$(QTDIR)/lib  -L/usr/X11R6/lib -lqt-mt -lpthread -lXext -lX11 -lm
CPPUNIT_INCLUDES = $(shell cppunit-config --cflags || echo -I../cppunit/include)
INCLUDES = $(CPPUNIT_INCLUDES) -I../src -I../src/qextserialport -I$(QTDIR)/include -I$(QTDIR)/mkspecs/default
CC = g++

SOURCES  = $(shell ls *.cpp)
TESTOBJECTS = $(shell echo $(SOURCES) | sed 's/main\.cpp//;s/mainqt\.cpp//;s/\.cpp/.o/g')
OBJECTS = $(TESTOBJECTS) $(shell find ../src -name "*.o" -not -name "main.o" -not -name "mainqt.o")

all: poa main

.PHONY : all clean check

.cpp.o:
	$(CC) $(CFLAGS) -c $(INCLUDES) -o $@ $<

../cppunit/lib/libcppunit.so:
	ln -s libcppunit-1.9.so.11.0.0 ../cppunit/lib/libcppunit.so

../cppunit/lib/libcppunit-1.9.so.11:
	ln -s libcppunit-1.9.so.11.0.0 ../cppunit/lib/libcppunit-1.9.so.11

../cppunit/lib/libqttestrunner.so:
	ln -s libqttestrunner.so.1.0.0 ../cppunit/lib/libqttestrunner.so

../cppunit/lib/libqttestrunner.so.1:
	ln -s libqttestrunner.so.1.0.0 ../cppunit/lib/libqttestrunner.so.1

main: ../cppunit/lib/libcppunit.so $(TESTOBJECTS) main.o
	$(CC) -o main $(LDFLAGS) $(OBJECTS) main.o

mainqt: ../cppunit/lib/libcppunit.so ../cppunit/lib/libqttestrunner.so $(TESTOBJECTS) mainqt.o
	$(CC) -o mainqt $(LDFLAGS) -lqttestrunner $(OBJECTS) mainqt.o

check: ../cppunit/lib/libcppunit-1.9.so.11 all
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):../cppunit/lib ./main

checkqt: ../cppunit/lib/libcppunit-1.9.so.11 ../cppunit/lib/libqttestrunner.so.1 all
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):../cppunit/lib ./mainqt

clean:
	rm *.o

depend:
	touch Makefile.depend
	makedepend -f Makefile.depend -- -I../src -- $(SOURCES)

poa:
	make -C ../src	

preparecheck:

ifeq ($(shell test -r Makefile.depend && echo "true"),true)
  include Makefile.depend
endif
