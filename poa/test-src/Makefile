CFLAGS   = -pipe -Wall -W -g -D_REENTRANT -D_TTY_POSIX_ -DQT_THREAD_SUPPORT
CXXFLAGS = -pipe -Wall -W -g -D_REENTRANT -D_TTY_POSIX_ -DQT_THREAD_SUPPORT
CPPUNIT_PATH = $(shell test $(shell hostname -s)=gspc54 && echo ../cppunit/lib/gspc54 || ../cppunit/lib)
CPPUNIT_LDFLAGS = $(shell cppunit-config --libs || echo -L$(CPPUNIT_PATH) -lcppunit)
LDFLAGS  = $(CPPUNIT_LDFLAGS) -L$(QTDIR)/lib  -L/usr/X11R6/lib -lqt-mt -lpthread -lXext -lX11 -lm
CPPUNIT_INCLUDES = $(shell cppunit-config --cflags || echo -I../cppunit/include)
INCLUDES = $(CPPUNIT_INCLUDES) -I../src -I../src/qextserialport -I$(QTDIR)/include -I$(QTDIR)/mkspecs/default
CC = gcc
CXX = g++

SOURCES  = $(shell ls *.cpp)
TESTOBJECTS = $(shell echo $(SOURCES) | sed 's/main\.cpp//;s/mainqt\.cpp//;s/\.cpp/.o/g')
OBJECTS = $(TESTOBJECTS) $(shell find ../src -name "*.o" -not -name "main.o" -not -name "mainqt.o")

all: main

.PHONY : all clean check

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) -o $@ $<

$(CPPUNIT_PATH)/libcppunit.so:
	ln -s libcppunit-1.9.so.11.0.0 $(CPPUNIT_PATH)/libcppunit.so

$(CPPUNIT_PATH)/libcppunit-1.9.so.11:
	ln -s libcppunit-1.9.so.11.0.0 $(CPPUNIT_PATH)/libcppunit-1.9.so.11

$(CPPUNIT_PATH)/libqttestrunner.so:
	ln -s libqttestrunner.so.1.0.0 $(CPPUNIT_PATH)/libqttestrunner.so

$(CPPUNIT_PATH)/libqttestrunner.so.1:
	ln -s libqttestrunner.so.1.0.0 $(CPPUNIT_PATH)/libqttestrunner.so.1

coverage: 
	make CXXFLAGS="$(CXXFLAGS) -fprofile-arcs -ftest-coverage" LDFLAGS="$(LDFLAGS) -fprofile-arcs -ftest-coverage" check

main: poa $(CPPUNIT_PATH)/libcppunit.so $(TESTOBJECTS) main.o
	$(CXX) -o main $(LDFLAGS) $(OBJECTS) main.o

mainqt: poa $(CPPUNIT_PATH)/libcppunit.so $(CPPUNIT_PATH)/libqttestrunner.so $(TESTOBJECTS) mainqt.o
	$(CXX) -o mainqt $(LDFLAGS) -lqttestrunner $(OBJECTS) mainqt.o

check: $(CPPUNIT_PATH)/libcppunit-1.9.so.11 all
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(CPPUNIT_PATH) ./main

checkqt: $(CPPUNIT_PATH)/libcppunit-1.9.so.11 $(CPPUNIT_PATH)/libqttestrunner.so.1 all
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(CPPUNIT_PATH) ./mainqt

clean:
	rm -f ../src/*.o ../src/qextserialbase/*.o	*.o

coverageclean:
	rm -f ../src/*.da ../src/qextserialbase/*.da *.da
	rm -f ../src/*.gcov ../src/qextserialbase/*.gcov *.gcov

mrproper: clean coverageclean
	rm -f ../src/*.bb ../src/qextserialbase/*.bb *.bb
	rm -f ../src/*.bbg ../src/qextserialbase/*.bbg *.bbg

depend:
	touch Makefile.depend
	makedepend -f Makefile.depend -- -I../src -- $(SOURCES)

gcover:
	cd ../src; find -name "*.cpp" -exec gcov {} \;
	for i in ../src/*.gcov; do test -e `echo $$i | sed -e s/\.gcov$$//` || rm $$i; done
	rm -f ../src/moc_*.gcov
	java -jar ../gcover/target/gcover-1.0-uber.jar ../src ../target/docs/gcover

poa:
	make -C ../src	

preparecheck:

ifeq ($(shell test -r Makefile.depend && echo "true"),true)
  include Makefile.depend
endif
