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
 * $Id: connectaction.h,v 1.2 2003/09/08 13:35:04 squig Exp $
 *
 *****************************************************************************/

#ifndef CONNECTACTION_H
#define CONNECTACTION_H

#include "canvasviewaction.h"
class CanvasView;
class PinView;

#include <qcanvas.h>
#include <qpoint.h>
class QMouseEvent;

/**
 * Provides an action that connects two {@link PinView} objects. Draws
 * a dahed line from the source pin to the current position of the
 * mouse.
 */
class ConnectAction : public CanvasViewAction
{

public:
    ConnectAction(CanvasView *view, QMouseEvent *e, PinView *source);
    virtual ~ConnectAction();

    /**
     *
     */
    virtual void mouseMoveEvent(QMouseEvent *e);

    /**
     * Deactivates the action.
     */
    virtual void mouseReleaseEvent(QMouseEvent *e);

private:
    /**
     * Returns the first active item in <code>items</code>.
     *
     * @return 0, if no item is found
     */
    QCanvasItem *activeItem(QCanvasItemList items);

    /**
     * Returns the active item that is within a limited bounding
     * rectangle around p. Tries items that directly collide with p
     * first.
     *
     * @return 0, if no item is active
     */
    QCanvasItem *activeItemAt(QPoint p);

    /** The source pin */
    PinView *source_;
    /** The old tracking state. */
    bool savedHasMouseTracking_;
    /** The dashed line. */
    QCanvasLine line_;
};

#endif // CONNECTACTION_H
