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
 * $Id: mdiwindow.h,v 1.20 2004/01/22 20:49:09 squig Exp $
 *
 *****************************************************************************/

#ifndef MDIWINDOW_H
#define MDIWINDOW_H

#include "project.h"
class GridCanvas;

#include <qmainwindow.h>
#include <qcanvas.h>
#include <qcombobox.h>
class CanvasView;

/**
 * Defines MDI windows.
 */
class MdiWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * Default constructor.
     */
    MdiWindow(CanvasView *view, QWidget* parent = 0, const char* name = 0,
              WFlags f = WType_TopLevel);

    /**
     * Default destructor.
     */
    ~MdiWindow();

    /**
     * Resizes the canvas, if its size changed according to this' zoom level
     */
    void resizeCanvas();

    /**
     * Returns the content of the window.
     */
    CanvasView *view() const;

    /**
     * Returns the current zoom level of this mdi window
     */
    double zoomLevel();

public slots:
    void setZoomLevel(double zoomLevel);

    /*
     * Modified the title caption if project has been modified.
     */
    void setModified(bool modified);

protected:
    /**
     * Gets called if sb want to close this mdi window
     */
    virtual void closeEvent(QCloseEvent *);

    /**
     * Gets called if sb resizes this mdi window
     */
    virtual void resizeEvent(QResizeEvent *e);

private:

    CanvasView *view_;
    double zoomLevel_;

};

#endif // MDIWINDOW_H

