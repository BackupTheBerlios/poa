LDFLAGS  = -L../cppunit/lib -lcppunit
INCLUDES = -I../cppunit/include
CC = g++
 
all: main

main.o: main.cpp
	$(CC) -c $(INCLUDES) main.cpp

example.o: example.cpp
	$(CC) -c $(INCLUDES) example.cpp

main: example.o main.o
	$(CC) -o main $(LDFLAGS) example.o main.o

check:
	test -L ../cppunit/lib/libcppunit-1.8.so.0 \
	 || ln -s libcppunit.so ../cppunit/lib/libcppunit-1.8.so.0
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):../cppunit/lib ./main

clean:
	rm *.o
