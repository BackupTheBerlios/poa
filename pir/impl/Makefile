CC=g++ -c
LINK=g++
INCLUDE_PATH=-I/home/garbeam/local/include/opencv -I/usr/include/gtk-2.0
LIBRARY_PATH=-L/home/garbeam/local/lib

ifeq ($(shell test -e Makefile.local && echo ja || echo nein),ja)
  include Makefile.local
endif

all      : test marcel motion manual

clean    : 
	rm -f test.o test marcel.o marcel motion.o motion steffen.o steffen

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

