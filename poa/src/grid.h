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
 * $Id: grid.h,v 1.1 2003/09/04 16:29:07 keulsn Exp $
 *
 *****************************************************************************/


#include <qpoint.h>

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

    virtual ~Grid();

    QPoint closestGridPoint(QPoint point);

private:
    double offsetX_, offsetY_;
    double gridSizeX_, gridSizeY_;
};
