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
 * $Id: mdiwindow.h,v 1.2 2003/08/20 08:38:56 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_MDIWINDOW_H
#define POA_MIDWINDOW_H

#include <qmainwindow.h>

#include "layoutcanvas.h"

/*****************************************************************************
 * Defines MDI windows.
 * @author garbeam
 */
class MdiWindow : public QMainWindow
{
//    Q_OBJECT

private:
    LayoutCanvas* layoutCanvas_;
    QString filename_;

public:
    MdiWindow( QWidget* parent, const char* name, int wflags );
    ~MdiWindow();

    void load( const QString& fn );
    void save();
    void saveAs();

protected:
    void closeEvent( QCloseEvent * );

//signals:
//    void message(const QString&, int );

};

#endif // POA_MDIWINDOW_H
