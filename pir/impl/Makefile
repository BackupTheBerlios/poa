CC=g++ -c
LINK=g++
INCLUDE_PATH=-I/home/garbeam/local/include/opencv
LIBRARY_PATH=-L/home/garbeam/local/lib

ifeq ($(shell test -e Makefile.local && echo ja || echo nein),ja)
  include Makefile.local
endif

all      : marcel steffen

clean    : 
	rm -f marcel.o marcel steffen.o steffen

marcel.o : marcel.c
	$(CC) marcel.c $(INCLUDE_PATH)

marcel   : marcel.o
	$(LINK) -o marcel marcel.o $(LIBRARY_PATH) -lstdc++ -lcv -lhighgui -lcxcore

steffen.o : steffen.c
	$(CC) steffen.c $(INCLUDE_PATH)

steffen   : steffen.o
	$(LINK) -o steffen steffen.o $(LIBRARY_PATH) -lstdc++ -lcv -lhighgui -lcxcore

