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
 * $Id: action.h,v 1.1 2003/11/01 20:07:46 squig Exp $
 *
 *****************************************************************************/

#ifndef CANVASVIEWACTION_H
#define CANVASVIEWACTION_H

class CanvasView;

class QMouseEvent;

/**
 * Defines the requirements for classes that provide a
 * {@link CanvasView} action.
 *
 * Each view has a single action that is active at a time. Actions
 * receive all mouse events from the view.
 */
class CanvasViewAction
{

public:
    CanvasViewAction(CanvasView *view);
    virtual ~CanvasViewAction();

    CanvasView *view();

    /**
     * Invoked when the action is cancelled, i.e. another action has
     * become active. Deletes the action.
     */
    virtual void cancel();

    /**
     * Invoked when the mouse cursor is moved over the view. Does nothing.
     */
    virtual void mouseMoveEvent(QMouseEvent *e);

    /**
     * Invoked when a mouse button is pressed the view. Does nothing.
     */
    virtual void mousePressEvent(QMouseEvent *e);

    /**
     * Invoked when a mouse button is released on the view. Does nothing.
     */
    virtual void mouseReleaseEvent(QMouseEvent *e);

private:
    CanvasView *view_;

};

#endif // CANVASVIEWACTION_H
