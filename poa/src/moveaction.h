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
 * $Id: moveaction.h,v 1.1 2003/09/07 19:07:46 squig Exp $
 *
 *****************************************************************************/

#ifndef MOVEACTION_H
#define MOVEACTION_H

#include "canvasviewaction.h"
class CanvasView;

#include <qpoint.h>
class QCanvasItem;
class QMouseEvent;

/**
 * Provides an action that moves an item within a {@link
 * CanvasView}. The item is snapped to the grid if the appropriate
 * setting is true.
 */
class MoveAction : public CanvasViewAction
{

public:
    MoveAction(CanvasView *view, QMouseEvent *e, QCanvasItem *item);

    /**
     * Moves the item to the new position.
     */
    virtual void mouseMoveEvent(QMouseEvent *e);

    /**
     * Deactivates the action.
     */
    virtual void mouseReleaseEvent(QMouseEvent *e);

private:
    /** The item that is currently in moving state. */
    QCanvasItem *item_;
    /** The point where the move started in canvas coordinates. */
    QPoint startPoint_;

};

#endif // MOVEACTION_H
