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
 * $Id: moveable.h,v 1.3 2004/01/22 22:27:47 squig Exp $
 *
 *****************************************************************************/


#ifndef MOVEABLE_H
#define MOVEABLE_H

#include <qpoint.h>
class QCanvasItem;

/**
 * Defines the requirements for movable view objects.
 */
class Moveable
{
public:

    /**
     * Drags <code>this</code> by <code>(dx, dy)</code>. <code>this</code>
     * will move itself by a distance close to that if possible.
     *
     * @param dx The horizontal coordinate of the distance along which
     *           <code>this</code> should be moved
     * @param dy The vertical coordinate of the distance along which
     *           <code>this</code> should be moved
     * @return The distance <code>this</code> has really moved. Does not
     *         have to be equal to <code>QSize (dx, dy)</code>.
     */
    virtual QPoint dragBy(int dx, int dy) = 0;

    virtual QCanvasItem *item() = 0;

};
#endif
