CC=gcc -c
LINK=g++
INCLUDE_PATH=/home/garbeam/local/include/opencv

all      : marcel

clean    : 
	rm -f marcel.o marcel

marcel.o : marcel.c
	$(CC) marcel.c -I$(INCLUDE_PATH)

marcel   : marcel.o
	$(LINK) -o marcel marcel.o -L/home/garbeam/local/lib -lstdc++ -lcv -lhighgui -lcxcore

