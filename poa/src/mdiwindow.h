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
 * $Id: mdiwindow.h,v 1.15 2003/08/28 15:31:10 vanto Exp $
 *
 *****************************************************************************/

#ifndef POA_MDIWINDOW_H
#define POA_MIDWINDOW_H

class GridCanvas;

#include "project.h"

#include <qmainwindow.h>
#include <qcanvas.h>
#include <qcombobox.h>

class CanvasView;

/*****************************************************************************
 * Defines MDI windows.
 */
class MdiWindow : public QMainWindow
{
    Q_OBJECT

public:
    MdiWindow(CanvasView *view, QWidget* parent = 0, const char* name = 0,
              WFlags f = WType_TopLevel);
    ~MdiWindow();

    QCanvas *canvas();
    void setCanvas(QCanvas *);

    void load( const QString& fn );
    void save();
    void saveAs();
    double zoomLevel();
    void resizeCanvas();

public slots:
    void setZoomLevel(double zoomLevel);

protected:
    void closeEvent( QCloseEvent * );
    virtual void resizeEvent(QResizeEvent *e);

private:
    CanvasView *view_;
    double zoomLevel_;
};

#endif // POA_MDIWINDOW_H
