CFLAGS   = -pipe -Wall -W -g -D_REENTRANT  -DQT_THREAD_SUPPORT
CXXFLAGS = -pipe -Wall -W -g -D_REENTRANT  -DQT_THREAD_SUPPORT
LDFLAGS  = -L../cppunit/lib -lcppunit -L$(QTDIR)/lib  -L/usr/X11R6/lib -lqt-mt -lpthread -lXext -lX11 -lm
INCLUDES = -I../cppunit/include -I../src -Iqextserialport -I$(QTDIR)/include -I$(QTDIR)/mkspecs/default
CC = g++

SOURCES  = *.cpp
//TESTOBJECTS = $(shell echo $(SOURCES) | sed 's/\.cpp/.o/')

TESTOBJECTS = pinmodeltest.o

OBJECTS = ../src/aboutdialog.o \
		../src/abstractmodel.o \
		../src/blockconfdialog.o \
		../src/blockmodel.o \
		../src/blockview.o \
		../src/canvasview.o \
		../src/canvasviewaction.o \
		../src/codemanager.o \
		../src/connectaction.o \
		../src/connectorrouter.o \
		../src/connectorviewlist.o \
		../src/connectorviewsegment.o \
		../src/cpumodel.o \
		../src/directrouter.o \
		../src/deployprojectwizard.o \
		../src/grid.o \
		../src/gridcanvas.o \
		../src/librarywindow.o \
		../src/mainwindow.o \
		../src/mdiwindow.o \
		../src/modelfactory.o \
		../src/moveaction.o \
		../src/muxconfdialog.o \
		../src/muxmodel.o \
		../src/muxview.o \
		../src/pinmodel.o \
		../src/pinview.o \
		../src/poa.o \
		../src/processdialog.o \
		../src/project.o \
		../src/reachedpoint.o \
		../src/settings.o \
		../src/settingsdialog.o \
		../src/util.o
OBJMOC = ../src/moc_aboutdialog.o \
		../src/moc_abstractmodel.o \
		../src/moc_blockconfdialog.o \
		../src/moc_blockmodel.o \
		../src/moc_blockview.o \
		../src/moc_canvasview.o \
		../src/moc_codemanager.o \
		../src/moc_connectorviewlist.o \
		../src/moc_connectorviewsegment.o \
		../src/moc_cpumodel.o \
		../src/moc_deployprojectwizard.o \
		../src/moc_gridcanvas.o \
		../src/moc_librarywindow.o \
		../src/moc_mainwindow.o \
		../src/moc_mdiwindow.o \
		../src/moc_muxconfdialog.o \
		../src/moc_muxmodel.o \
		../src/moc_pinmodel.o \
		../src/moc_pinview.o \
		../src/moc_processdialog.o \
		../src/moc_project.o \
		../src/moc_settings.o \
		../src/moc_settingsdialog.o

all: main

.PHONY : all clean check

.cpp.o:
	$(CC) $(CFLAGS) -c $(INCLUDES) -o $@ $<

main: $(OBJECTS) $(OBJMOC) $(TESTOBJECTS) main.o
	$(CC) -o main $(LDFLAGS) $(OBJECTS) $(OBJMOC) $(TESTOBJECTS) main.o

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
