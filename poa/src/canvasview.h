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
 * $Id: canvasview.h,v 1.30 2004/01/26 15:57:17 squig Exp $
 *
 *****************************************************************************/

#ifndef CANVASVIEW_H
#define CANVASVIEW_H

class AbstractModel;
class AbstractView;
class CanvasToolTip;
class CanvasViewAction;
class GridCanvas;
class Project;
class PinView;

#include <qvariant.h>
#include <qcanvas.h>
#include <qtooltip.h>
class QCanvasItemList;
class QCanvasItem;
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
class QKeyEvent;
class QPoint;
class QPopupMenu;
class QToolTip;

/**
 * Defines a view on a {@link QCanvas} that belongs to a {@link Project}.
 * Each project can have several <code>CanvasView</code>s. Several
 * <code>CanvasView</code>s can visualize one <code>QCanvas</code>.
 */
class CanvasView : public QCanvasView
{
    Q_OBJECT

public:

    enum EditMode { Default, Annotate };

    /**
     * Creates a new <code>CanvasView</code>
     * @param project The project this canvas view belongs to
     * @param canvas see {@link QCanvasView#QCanvasView}
     * @param parent see {@link QCanvasView#QCanvasView}
     * @param name see {@link QCanvasView#QCanvasView}
     * @param fl see {@link QCanvasView#QCanvasView}
     */
    CanvasView(Project *project, GridCanvas *canvas, QWidget *parent = 0,
               const char* name = 0, WFlags fl = 0);

    /**
     * Default destructor
     */
    ~CanvasView();

    EditMode editMode() const;

    /**
     * Returns the associated canvas.
     */
    GridCanvas *gridCanvas();

    /**
     * Returns the project <code>this</code> belongs to.
     */
    Project *project();

    /**
     * Returns a list of the selected items.
     */
    QCanvasItemList selectedItems();

    /**
     * Sets the action for the CanvasView. The currently active action
     * is cancelled.
     */
    void setAction(CanvasViewAction *action);

    void setEditMode(EditMode mode);

    virtual void timerEvent(QTimerEvent *e);

public slots:
    /**
     * Deselects all selected items.  */
    void deselectAll();

signals:
    /**
     * Emitted when the selection changes.
     *
     * @param item, the item that was seleted; 0, if no item is selected
     */
    void selectionChanged(QCanvasItem *item);

protected:

    /**
     * Called when a mouse button is pressed on <code>this</code>'s content.
     * If the mouse button is pressed on a descendant of {@link BlockView}
     * then sets that object as <code>movingItem_</code>.
     */
    virtual void contentsMousePressEvent(QMouseEvent *e);

    /**
     * Called when a mouse button is released on <code>this</code>'s content.
     * Cleares the moving state, if an item exists that has movin state.
     */
    virtual void contentsMouseReleaseEvent(QMouseEvent *e);

    /**
     * Called when the mouse cursor is moved over <code>this</code>'s content.
     * If there is a <code>movingItem_</code> then that item is moved along
     * with the cursor. The current position is displayed in the
     * status bar.
     */
    virtual void contentsMouseMoveEvent(QMouseEvent *e);

    /**
     * Called when a mouse button is double clicked on
     * <code>this</code>'s content.
     */
    virtual void contentsMouseDoubleClickEvent(QMouseEvent *e);


    /**
     * Approves those <code>QDragEnterEvent</code> that may be passed to
     * {@link #dropEvent}.
     */
    virtual void dragEnterEvent(QDragEnterEvent *e);

    /**
     * Can only be used for those <code>QDropEvents</code> approved by
     * {@link #dragEnterEvent}. Tries to create a new model from the dragged
     * content an adds that model to the project. Then creates a new view
     * for that model and adds that view to the canvas <code>this</code> is
     * based on.
     */
    virtual void dropEvent(QDropEvent *e);

    /**
     * Sets the selected state of item to true. All other items are
     * deselected and a selectionChanged signal is emitted.
     *
     * Only single selection is supported for now.
     */
    void selectItem(QCanvasItem *item);

    /**
     * Transforms window-coordinates to canvas-coodinates
     * @param pos Position in window-coodinates
     * @return Position in canvas-world-coodinates
     */
    QPoint toCanvas(QPoint pos);

private:
    /**
     * Auto scrolls the canvas if the mouse cursor is moved within
     * AUTOSCROLL_MARGIN.
     */
    void doAutoScroll();

    /** The project this view belongs to */
    Project *project_;
    /** The action that is currently active if any, else 0. */
    CanvasViewAction *action_;
    /** The Tooltip instance. */
    CanvasToolTip *tooltip_;
    /** The popup menu. */
    QPopupMenu *popupMenu;

    /**
     * The edit mode. Default means standard selektion of blocks,
     * Annotate means adding annontations with an IBeamCursor.
     */
    EditMode editMode_;

    int autoScrollTimerId;
};

class CanvasToolTip : public QToolTip
{
 public:
    CanvasToolTip(QCanvasView *parent);

 protected:
    void maybeTip(const QPoint &);
};

#endif // CANVASVIEW_H
