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
 * $Id: connectormoveaction.h,v 1.1 2004/01/28 01:49:37 kilgus Exp $
 *
 *****************************************************************************/

#ifndef CONNECTORMOVEACTION_H
#define CONNECTORMOVEACTION_H

#include "canvasviewaction.h"
class CanvasView;
class ConnectorViewSegment;

#include <qpoint.h>
class QMouseEvent;

/**
 * Provides an action that moves a connector item within a CanvasView. 
 */
class ConnectorMoveAction : public CanvasViewAction
{
public:
    ConnectorMoveAction(CanvasView *view, QMouseEvent *e, ConnectorViewSegment *item);

    /**
     * Moves the item to the new position.
     */
    virtual void mouseMoveEvent(QMouseEvent *e);

    /**
     * Deactivates the action.
     */
    virtual void mouseReleaseEvent(QMouseEvent *e);

    /**
     * Drags the segment and adds/removes segments as needed
     */
    virtual QPoint dragBy(int dx, int dy);

private:
    /** The segment that is currently in moving state. */
    ConnectorViewSegment *item_;
    /** The point where the move started in canvas coordinates. */
    QPoint startPoint_;
};

#endif // CONNECTORMOVEACTION_H