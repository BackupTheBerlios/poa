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
 * $Id: main.cpp,v 1.11 2003/08/22 11:53:40 garbeam Exp $
 *
 *****************************************************************************/

// NOTE:
//   The following model-/view-headers are included temporary to
//   check, if everything compiles.
//   Later this won't be needed anymore.
//
#include "blockmodel.h"
#include "blockview.h"
#include "codemanager.h"
#include "connector.h"
#include "connectormodel.h"
#include "connectorview.h"
#include "coremodel.h"
#include "coreview.h"
#include "cpumodel.h"
#include "cpuview.h"
#include "inputblockmodel.h"
#include "inputblockview.h"
#include "layoutmanager.h"
#include "mainwindow.h"
#include "moduleconfdialog.h"
#include "modulelibraryitem.h"
#include "muxmodel.h"
#include "muxview.h"
#include "outputblockmodel.h"
#include "outputblockview.h"
#include "pinmodel.h"
#include "pinvector.h"
#include "pinview.h"
#include "viewmap.h"
// END of temporary inclusions
//

// find a good place for constants, maybe consts.h?
#define VERSION "POA-current"

#include "mainwindow.h"
#include <qapplication.h>
#include <iostream.h>
#include <qdom.h>

/**
 * This is the main entry point of the POA app.
 * @author garbeam
 */
int main (int argc, char *argv[])
{
    CpuModel model(new QString("CPU 1"), 1, TRUE);
    QByteArray xml = model.serialize();
    cout << xml;

    QDomDocument *doc = new QDomDocument("cpu");
    doc->setContent(xml);
    CpuModel model2(doc);

    xml = model.serialize();
    cout << xml;
    
    QApplication app(argc, argv);
    MainWindow *mainWindow = new MainWindow();
    app.setMainWidget(mainWindow);
    mainWindow->setCaption(VERSION);
    mainWindow->show();

    return app.exec();
}

