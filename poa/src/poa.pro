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
# $Id: poa.pro,v 1.18 2003/08/22 15:06:11 squig Exp $
#
###############################################################################

CONFIG += debug

TEMPLATE = app
INCLUDEPATH += .

# added for use under OpenBSD (garbeam)
UNAME = $$system(uname -s)
contains (UNAME, OpenBSD) {
	# I have multi-threaded Qt
	INCLUDEPATH += $(QTINC)
	CONFIG += thread
}

# source files
HEADERS += aboutdialog.h 
HEADERS += abstractmodel.h
HEADERS += blockmodel.h
HEADERS += blockview.h
HEADERS += canvasview.h
HEADERS += codemanager.h
HEADERS += connectormodel.h
HEADERS += connectorview.h
HEADERS += coremodel.h
# HEADERS += coreview.h
HEADERS += cpumodel.h
HEADERS += cpuview.h
HEADERS += inputblockmodel.h
HEADERS += inputblockview.h
HEADERS += layoutmanager.h
HEADERS += librarywindow.h
HEADERS += mainwindow.h
HEADERS += mdiwindow.h
HEADERS += moduleconfdialog.h 
HEADERS += muxmodel.h
HEADERS += muxview.h
HEADERS += settingsdialog.h
HEADERS += outputblockmodel.h
HEADERS += outputblockview.h
HEADERS += pinmodel.h
HEADERS += pinvector.h
HEADERS += pinview.h
HEADERS += settings.h
HEADERS += viewmap.h
HEADERS += gridcanvas.h

SOURCES += aboutdialog.cpp
SOURCES += blockview.cpp
SOURCES += canvasview.cpp
SOURCES += cpumodel.cpp
SOURCES += gridcanvas.cpp
SOURCES += librarywindow.cpp
SOURCES += main.cpp
SOURCES += mainwindow.cpp
SOURCES += mdiwindow.cpp
SOURCES += moduleconfdialog.cpp
SOURCES += settings.cpp
SOURCES += settingsdialog.cpp

TARGET = poa

