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
 * $Id: canvasview.h,v 1.13 2003/08/28 16:10:29 keulsn Exp $
 *
 *****************************************************************************/
#ifndef CANVASVIEW_H
#define CANVASVIEW_H

class AbstractModel;
class Project;

#include <qvariant.h>
#include <qcanvas.h>
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
class QPoint;

/*****************************************************************************
 * Defines a view on a {@link QCanvas} that belongs to a {@link Project}.
 * Each project can have several <code>CanvasView</code>s. Several
 * <code>CanvasView</code>s can visualize one <code>QCanvas</code>.
 */
class CanvasView : public QCanvasView
{
    Q_OBJECT

public:

    /*************************************************************************
     * Creates a new <code>CanvasView</code>
     * @param project The project this canvas view belongs to
     * @param canvas see {@link QCanvasView#QCanvasView}
     * @param parent see {@link QCanvasView#QCanvasView}
     * @param name see {@link QCanvasView#QCanvasView}
     * @param fl see {@link QCanvasView#QCanvasView}
     */
    CanvasView(Project *project, QCanvas *canvas, QWidget *parent = 0,
               const char* name = 0, WFlags fl = 0);

    /*************************************************************************
     * Default destructor
     */
    ~CanvasView();

    /*************************************************************************
     * Returns the project <code>this</code> belongs to.
     */
    Project *project();

    /*************************************************************************
     * Called when a mouse button is pressed on <code>this</code>'s content.
     * If the mouse button is pressed on a descendant of {@link BlockView}
     * then sets that object as <code>movingItem_</code>.
     */
    void contentsMousePressEvent(QMouseEvent *e);

    /*************************************************************************
     * Called when the mouse cursor is moved over <code>this</code>'s content.
     * If there is a <code>movingItem_</code> then that item is moved along
     * with the cursor. The current position is displayed in the
     * status bar.
     */
    void contentsMouseMoveEvent(QMouseEvent *e);

    /*************************************************************************
     * Approves those <code>QDragEnterEvent</code> that may be passed to
     * {@link #dropEvent}.
     */
    void dragEnterEvent(QDragEnterEvent *e);

    /*************************************************************************
     * Can only be used for those <code>QDropEvents</code> approved by
     * {@link #dragEnterEvent}. Tries to create a new model from the dragged
     * content an adds that model to the project. Then creates a new view
     * for that model and adds that view to the canvas <code>this</code> is
     * based on.
     */
    void dropEvent(QDropEvent *e);

    /*************************************************************************
     * Transforms window-coordinates to canvas-coodinates
     * @param pos Position in window-coodinates
     * @return Position in canvas-world-coodinates
     */
    QPoint toCanvas(QPoint pos);

private:
    /** The project this view belongs to */
    Project *project_;
    /** The canvas item that is currently in moving state if any, else 0 */
    QCanvasItem *movingItem_;
    /**
     * The point in <>-coordinates where <code>movingItem_</code>
     * received the moving state. Only valid if <code>movingItem_ != 0</code>
     */
    QPoint movingStartPoint_;
};

#endif // CANVASVIEW_H
