###############################################################################
# qmake definition
###############################################################################
# POA - Programmierumgebung Offener Antrieb
# Copyright (C) 2003 Necati Aydin, Anselm Garbe, Stefan Hauser,
# Steffen Keul, Marcel Kilgus, Steffen Pingel, Tammo van Lessen
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#
# $Id: poa.pro,v 1.79 2004/01/28 01:49:37 kilgus Exp $
#
###############################################################################

CONFIG += debug 
CONFIG += release
CONFIG += thread
CONFIG += rtti
CONFIG += exceptions

TEMPLATE = app
INCLUDEPATH += .
INCLUDEPATH += ./qextserialport

win32:OBJECTS_DIR = debug

TARGET = poa
QMAKE_TARGET_DESTDIR = poa-0.1.0
QMAKE_DEL_DIR = rm -rf

DISTFILES += AUTHORS BUGS COPYING README TODO about.html 
DISTFILES += templates/*.c icons/*.xpm icons/*.png

include(poa.local)

# source files
HEADERS += aboutdialog.h 
HEADERS += abstractmodel.h
HEADERS += blockconfdialog.h 
HEADERS += blockgraph.h
HEADERS += blockmodel.h
HEADERS += blockview.h
HEADERS += canvasview.h
HEADERS += canvasviewaction.h
HEADERS += codemanager.h
HEADERS += colormanager.h
HEADERS += connectaction.h
HEADERS += connectorrouter.h
HEADERS += connectorviewlist.h
HEADERS += connectorviewsegment.h
HEADERS += connectormoveaction.h
HEADERS += copyable.h
HEADERS += cpumodel.h
HEADERS += dijkstrarouter.h
HEADERS += directrouter.h
HEADERS += deployprojectwizard.h
HEADERS += downloadmanager.h
HEADERS += genericpriorityqueue.h
HEADERS += grid.h
HEADERS += gridcanvas.h
HEADERS += layoutmanager.h
HEADERS += librarywindow.h
HEADERS += mainwindow.h
HEADERS += mdiwindow.h
HEADERS += modelfactory.h
HEADERS += moveable.h
HEADERS += moveaction.h
HEADERS += muxconfdialog.h
HEADERS += muxmodel.h
HEADERS += pinlistviewitem.h
HEADERS += pinmodel.h
HEADERS += pinview.h
HEADERS += pixmapbutton.h
HEADERS += poa.h
HEADERS += poaexception.h
HEADERS += printmanager.h
HEADERS += priorityqueue.h
HEADERS += problemmanager.h
HEADERS += processdialog.h
HEADERS += project.h
HEADERS += removeable.h
HEADERS += router.h
HEADERS += scheduledialog.h
HEADERS += serializable.h
HEADERS += settings.h
HEADERS += settingsdialog.h
HEADERS += textview.h
HEADERS += tooltipable.h
HEADERS += util.h

SOURCES += aboutdialog.cpp
SOURCES += abstractmodel.cpp
SOURCES += blockconfdialog.cpp
SOURCES += blockmodel.cpp
SOURCES += blockgraph.cpp
SOURCES += blockview.cpp
SOURCES += canvasview.cpp
SOURCES += canvasviewaction.cpp
SOURCES += codemanager.cpp
SOURCES += colormanager.cpp
SOURCES += connectaction.cpp
SOURCES += connectorrouter.cpp
SOURCES += connectorviewlist.cpp
SOURCES += connectorviewsegment.cpp
SOURCES += connectormoveaction.cpp
SOURCES += cpumodel.cpp
SOURCES += dijkstrarouter.cpp
SOURCES += directrouter.cpp
SOURCES += deployprojectwizard.cpp
SOURCES += downloadmanager.cpp
SOURCES += grid.cpp
SOURCES += gridcanvas.cpp
SOURCES += librarywindow.cpp
SOURCES += main.cpp
SOURCES += mainwindow.cpp
SOURCES += mdiwindow.cpp
SOURCES += modelfactory.cpp
SOURCES += moveaction.cpp
SOURCES += muxconfdialog.cpp
SOURCES += muxmodel.cpp
SOURCES += pinlistviewitem.cpp
SOURCES += pinmodel.cpp
SOURCES += pinview.cpp
SOURCES += pixmapbutton.cpp
SOURCES += poa.cpp
SOURCES += poaexception.cpp
SOURCES += printmanager.cpp
SOURCES += priorityqueue.cpp
SOURCES += problemmanager.cpp
SOURCES += processdialog.cpp
SOURCES += project.cpp
SOURCES += scheduledialog.cpp
SOURCES += settings.cpp
SOURCES += settingsdialog.cpp
SOURCES += textview.cpp
SOURCES += util.cpp

###############################################################################
# Qextserialport
##############################################################################
HEADERS += qextserialport/qextserialbase.h
HEADERS += qextserialport/qextserialport.h

SOURCES += qextserialport/qextserialbase.cpp
SOURCES += qextserialport/qextserialport.cpp

unix:HEADERS += qextserialport/posix_qextserialport.h
unix:SOURCES += qextserialport/posix_qextserialport.cpp
unix:DEFINES  = _TTY_POSIX_

win32:HEADERS += qextserialport/win_qextserialport.h
win32:SOURCES += qextserialport/win_qextserialport.cpp
win32:DEFINES = _TTY_WIN_

