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
 * $Id: grid.h,v 1.4 2004/01/12 02:37:27 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA__GRID_H
#define POA__GRID_H

#include <qpoint.h>

#include "poa.h"

class Grid
{
public:

    /**
     * Creates a grid with rectangular grid size.
     *
     * @param start One grid point
     * @param gridSize Width = Height of the rectangle spanned by adjacent
     *        grid points. Must be greater than 0
     */
    Grid(QPoint start, int gridSize);
    
    /**
     * Creates a grid that fits onto two points and uses a magnitude of the
     * grid size. The resulting grid might have different width and height
     * in a rectangle spanned by adjacent grid points.
     *
     * @param first One grid point
     * @param second Another grid point, must not be equal to first
     * @param gridMagnitude Magnitude for the grid size. A grid size
     *        closest to <code>gridMagnitude</code> that satisfies
     *        <code>first + n * gridSize = second</code> is chosen.
     */
    Grid(QPoint first, QPoint second, int gridMagnitude);

    /**
     * Default destructor
     */
    virtual ~Grid();

    /**
     * Returns a grid point with minimal distance to <code>point</code>
     */
    QPoint closestGridPoint(QPoint point) const;

    /**
     * Returns the grid point reached by moving <code>point</code> by
     * <code>x</code> times grid size horizontally and <code>y</code> times
     * grid size vertically.
     */
    QPoint move(QPoint point, int x, int y) const;
    
    /**
     * Returns the grid point reached by moving <code>point</code> by
     * <code>distance</code> times grid size in direction <code>dir</code>
     */
    QPoint move(QPoint point, LineDirection dir, int distance) const;

    /**
     * Calculates the number of grid steps between two grid points.
     *
     * param start A grid point
     * param end Another grid point
     * param x Reference to an integer variable. The number of horizontal steps
     *       will be saved in <code>x</code>
     * param y Reference to an integer variable. The number of vertical steps
     *       will be saved in <code>y</code>
     */
    void getGridDistance(QPoint start, QPoint end, int &x, int &y) const;

private:
    double offsetX_, offsetY_;
    double gridSizeX_, gridSizeY_;
};


#endif // POA__GRID_H
