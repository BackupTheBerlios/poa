SOURCES  = *.cpp
OBJECTS  = $(shell echo $(SOURCES) | sed 's/\.cpp/.o/')
LDFLAGS  = -L../cppunit/lib -lcppunit
INCLUDES = -I../cppunit/include -I../src
CC = g++
CFLAGS = -Wall $(INCLUDES)

OBJECT   = 

%.o : %.cpp
	$(CC) $(CFLAGS) -c $<

.PHONY : all clean check

all: main

main: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o main $(OBJECTS)

check: all
	test -L ../cppunit/lib/libcppunit-1.8.so.0 \
	 || ln -s libcppunit.so ../cppunit/lib/libcppunit-1.8.so.0
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):../cppunit/lib ./main

clean:
	rm *.o

depend:
	touch Makefile.depend
	makedepend -f Makefile.depend -- -I../src -- $(SOURCES)

ifeq ($(shell test -r Makefile.depend && echo "true"),true)
  include Makefile.depend
endif
