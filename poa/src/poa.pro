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
# $Id: poa.pro,v 1.56 2003/11/26 14:31:32 garbeam Exp $
#
###############################################################################

#CONFIG += debug 
CONFIG += release
CONFIG += thread

TEMPLATE = app
INCLUDEPATH += .

TARGET = poa
QMAKE_TARGET_DESTDIR = poa-0.1.0
QMAKE_DEL_DIR = rm -rf
#QMAKE_CXXFLAGS = -DVERSION=0.1.0

DISTFILES += AUTHORS BUGS COPYING README TODO about.html 
DISTFILES += templates/*.c icons/*.xpm icons/*.png

include(poa.local)

# source files
HEADERS += aboutdialog.h 
HEADERS += abstractmodel.h
HEADERS += blockmodel.h
HEADERS += blockview.h
HEADERS += canvasview.h
HEADERS += canvasviewaction.h
HEADERS += codemanager.h
HEADERS += connectaction.h
HEADERS += connectorviewlist.h
HEADERS += connectorviewsegment.h
HEADERS += cpumodel.h
HEADERS += grid.h
HEADERS += layoutmanager.h
HEADERS += librarywindow.h
HEADERS += mainwindow.h
HEADERS += mdiwindow.h
HEADERS += modelfactory.h
HEADERS += blockconfdialog.h 
HEADERS += moveaction.h
HEADERS += muxconfdialog.h
HEADERS += muxmodel.h
HEADERS += muxview.h
HEADERS += settingsdialog.h
HEADERS += pinmodel.h
HEADERS += pinview.h
HEADERS += processdialog.h
HEADERS += project.h
HEADERS += poa.h
HEADERS += reachedpoint.h
HEADERS += serializable.h
HEADERS += settings.h
HEADERS += gridcanvas.h
HEADERS += tooltipable.h
HEADERS += util.h
HEADERS += deployprojectwizard.h

SOURCES += aboutdialog.cpp
SOURCES += abstractmodel.cpp
SOURCES += blockmodel.cpp
SOURCES += blockview.cpp
SOURCES += canvasview.cpp
SOURCES += canvasviewaction.cpp
SOURCES += codemanager.cpp
SOURCES += connectaction.cpp
SOURCES += connectorviewlist.cpp
SOURCES += connectorviewsegment.cpp
SOURCES += cpumodel.cpp
SOURCES += grid.cpp
SOURCES += gridcanvas.cpp
SOURCES += librarywindow.cpp
SOURCES += main.cpp
SOURCES += mainwindow.cpp
SOURCES += mdiwindow.cpp
SOURCES += blockconfdialog.cpp
SOURCES += modelfactory.cpp
SOURCES += moveaction.cpp
SOURCES += muxconfdialog.cpp
SOURCES += muxmodel.cpp
SOURCES += muxview.cpp
SOURCES += pinmodel.cpp
SOURCES += pinview.cpp
SOURCES += processdialog.cpp
SOURCES += project.cpp
SOURCES += reachedpoint.cpp
SOURCES += settings.cpp
SOURCES += settingsdialog.cpp
SOURCES += util.cpp
SOURCES += deployprojectwizard.cpp

