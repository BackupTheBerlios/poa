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
# $Id: poa.pro,v 1.32 2003/09/11 12:43:11 garbeam Exp $
#
###############################################################################

CONFIG += debug thread

TEMPLATE = app
INCLUDEPATH += .

include(poa.local)

# source files
HEADERS += aboutdialog.h 
HEADERS += abstractmodel.h
HEADERS += abstractview.h
HEADERS += blockmodel.h
HEADERS += blockview.h
HEADERS += canvasview.h
HEADERS += canvasviewaction.h
HEADERS += codemanager.h
HEADERS += connectaction.h
HEADERS += connectormodel.h
HEADERS += connectorview.h
HEADERS += coremodel.h
HEADERS += coreview.h
HEADERS += cpumodel.h
HEADERS += cpuview.h
HEADERS += inputblockmodel.h
HEADERS += inputblockview.h
HEADERS += layoutmanager.h
HEADERS += librarywindow.h
HEADERS += mainwindow.h
HEADERS += mdiwindow.h
HEADERS += modelfactory.h
HEADERS += blockconfdialog.h 
HEADERS += moveaction.h
HEADERS += muxmodel.h
HEADERS += muxview.h
HEADERS += settingsdialog.h
HEADERS += outputblockmodel.h
HEADERS += outputblockview.h
HEADERS += pinmodel.h
HEADERS += pinvector.h
HEADERS += pinview.h
HEADERS += project.h
HEADERS += poa.h
HEADERS += settings.h
HEADERS += gridcanvas.h
HEADERS += tooltipable.h

SOURCES += aboutdialog.cpp
SOURCES += abstractmodel.cpp
SOURCES += blockmodel.cpp
SOURCES += blockview.cpp
SOURCES += canvasview.cpp
SOURCES += canvasviewaction.cpp
SOURCES += connectaction.cpp
SOURCES += connectormodel.cpp
SOURCES += connectorview.cpp
SOURCES += coremodel.cpp
SOURCES += coreview.cpp
SOURCES += cpumodel.cpp
SOURCES += cpuview.cpp
SOURCES += gridcanvas.cpp
SOURCES += librarywindow.cpp
SOURCES += main.cpp
SOURCES += mainwindow.cpp
SOURCES += mdiwindow.cpp
SOURCES += blockconfdialog.cpp
SOURCES += modelfactory.cpp
SOURCES += moveaction.cpp
SOURCES += pinmodel.cpp
SOURCES += pinvector.cpp
SOURCES += pinview.cpp
SOURCES += project.cpp
SOURCES += settings.cpp
SOURCES += settingsdialog.cpp

TARGET = poa

