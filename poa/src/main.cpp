/*****************************************************************************
 * POA - Programmierumgebung Offener Antrieb
 *
 * Copyright (C) 2003 Necati Aydin, Anselm Garbe, Stefan Hauser,
 * Steffen Keul, Marcel Kilgus, Steffen Pingel, Tammo van Lessen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * $Id: main.cpp,v 1.18 2003/09/07 19:07:46 squig Exp $
 *
 *****************************************************************************/

// find a good place for constants, maybe consts.h?
#define VERSION "POA-current"

#include "mainwindow.h"

#include <qapplication.h>
#include <qdom.h>

/**
 * This is the main entry point of the POA app.
 * @author garbeam
 */
int main (int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow *mainWindow = new MainWindow();
    app.setMainWidget(mainWindow);
    mainWindow->setCaption(VERSION);
    mainWindow->show();

    return app.exec();
}

