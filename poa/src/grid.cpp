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
 * $Id: grid.cpp,v 1.1 2003/09/04 16:29:07 keulsn Exp $
 *
 *****************************************************************************/


#include "grid.h"


Grid::Grid(QPoint start, int gridSize)
{
    offsetX_ = (double) start.x();
    offsetY_ = (double) start.y();
    gridSizeX_ = (double) gridSize;
    gridSizeY_ = (double) gridSize;
}


Grid::Grid(QPoint first, QPoint second, int gridMagnitude)
{
    Q_ASSERT(second.x() != first.x() && second.y() != first.y());
    offsetX_ = (double) first.x();
    offsetY_ = (double) first.y();

    double delta = (double) (second.x() - first.x());
    int n = (int) (0.5 + delta / ((double) gridMagnitude));
    gridSizeX_ = delta / ((double) n);

    delta = (double) (second.y() - first.y());
    n = (int) (0.5 + delta / ((double) gridMagnitude));
    gridSizeY_ = delta / ((double) n);
}


Grid::~Grid()
{
}


QPoint Grid::closestGridPoint(QPoint point)
{
    // offset + n * gridSize - point = min
    int x = (int) (0.5 + ((double) point.x() - offsetX_) / gridSizeX_);
    int y = (int) (0.5 + ((double) point.y() - offsetY_) / gridSizeY_);
    return QPoint((int) (offsetX_ + ((double) x) * gridSizeX_),
		  (int) (offsetY_ + ((double) y) * gridSizeY_));
}
