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
 * $Id: abstractview.h,v 1.11 2003/09/11 13:17:14 squig Exp $
 *
 *****************************************************************************/


#ifndef ABSTRACTVIEW_H
#define ABSTRACTVIEW_H

class AbstractModel;
class CanvasView;

#include <qobject.h>
class QMouseEvent;
class QPopupMenu;
#include <qsize.h>

/**
 * Base class for all view classes.
 */
class AbstractView
{
public:

    /**
     * Drags <code>this</code> by <code>(dx, dy)</code>. <code>this</code>
     * will move itself by a distance close to that if possible.
     *
     * Note that <code>this</code> will never move if
     * !{@link #isDraggable()}
     *
     * @param dx The horizontal coordinate of the distance along which
     *           <code>this</code> should be moved
     * @param dy The vertical coordinate of the distance along which
     *           <code>this</code> should be moved
     * @return The distance <code>this</code> has really moved. Does not
     *         have to be equal to <code>QSize (dx, dy)</code>.
     */
    virtual QSize dragBy(double, double) { return QSize(0, 0); }

    /**
     * Returns <code>true</code> if <code>this</code> can be dragged using
     * the method {@link #drag}, <code>false</code> else.
     */
    virtual bool isDraggable() { return false; };

    /**
     * Returns the model.
     */
    virtual AbstractModel *model() = 0;

};
#endif
