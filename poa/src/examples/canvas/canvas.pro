# added for local use under BSD
INCLUDEPATH	+= $(QTINC)

TEMPLATE	= app
CONFIG		+= qt warn_on release thread
HEADERS		= canvas.h
SOURCES		= canvas.cpp main.cpp
TARGET		= canvas

