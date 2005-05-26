CC=g++ -c
LINK=g++
INCLUDE_PATH=-I/usr/include/opencv -I/usr/include/gtk-2.0
LIBRARY_PATH=-L/usr/lib

ifeq ($(shell test -e Makefile.local && echo ja || echo nein),ja)
  include Makefile.local
endif

all      : test marcel motion manual setfeature pyramid

clean    : 
	rm -f *.o test marcel motion steffen pyramid setfeature

test.o   : test.c
	$(CC) test.c $(INCLUDE_PATH)

test     : test.o
	$(LINK) -o test test.o $(LIBRARY_PATH) -lstdc++ -lcv -lhighgui -l cxcore

marcel.o : marcel.c
	$(CC) marcel.c $(INCLUDE_PATH)

marcel   : marcel.o
	$(LINK) -o marcel marcel.o $(LIBRARY_PATH) -lstdc++ -lcv -lhighgui -lcxcore

motion.o  : motion.c
	$(CC) -g motion.c $(INCLUDE_PATH)

motion    : motion.o
	$(LINK) -g -o motion motion.o $(LIBRARY_PATH) -lstdc++ -lcv -lhighgui -lcxcore

manual.o : manual.c
	$(CC) manual.c $(INCLUDE_PATH)

manual   : manual.o
	$(LINK) -o manual manual.o $(LIBRARY_PATH) -lstdc++ -lcv -lhighgui -lcxcore

setfeature.o : setfeature.c features.h
	$(CC) setfeature.c -o setfeature.o $(INCLUDE_PATH)

setfeature   : setfeature.o
	$(LINK) -o setfeature setfeature.o $(LIBRARY_PATH) -lstdc++ -lcv -lhighgui -lcxcore

pyramid.o : pyramid.c features.h
	$(CC) pyramid.c -o pyramid.o $(INCLUDE_PATH)

pyramid   : pyramid.o
	$(LINK) -o pyramid pyramid.o $(LIBRARY_PATH) -lstdc++ -lcv -lhighgui -lcxcore

