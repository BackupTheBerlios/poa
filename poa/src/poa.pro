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
# $Id: poa.pro,v 1.3 2003/08/20 08:38:56 garbeam Exp $
#
###############################################################################

TEMPLATE = app
INCLUDEPATH += .

# added for use under BSD and some Linux distris (garbeam)
#INCLUDEPATH	+= $(QTINC)

# source files
HEADERS +=  blockmodel.h blockview.h codemanager.h connectormodel.h connectorview.h coremodel.h coreview.h cpumodel.h cpuview.h inputblockmodel.h inputblockview.h layoutcanvas.h layoutmanager.h mainwindow.h mdiwindow.h moduleconfdialog.h modulelibraryitem.h muxmodel.h muxview.h optiondialog.h outputblockmodel.h outputblockview.h pinmodel.h pinvector.h pinview.h viewmap.h
SOURCES += mainwindow.cpp moduleconfdialog.cpp optiondialog.cpp main.cpp

TARGET = poa
