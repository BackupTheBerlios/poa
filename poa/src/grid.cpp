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
 * $Id: grid.cpp,v 1.8 2004/01/12 02:37:27 keulsn Exp $
 *
 *****************************************************************************/


#include <math.h>

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
    double delta;
    int n;

    if (first == second) {
	Q_ASSERT(false);
	gridSizeX_ = 0.0;
	gridSizeY_ = 0.0;
    }

    bool ownX = second.x() != first.x();
    offsetX_ = (double) first.x();
    bool ownY = second.y() != first.y();
    offsetY_ = (double) first.y();

    if (ownX) {
	delta = (double) (second.x() - first.x());
	n = (int) rint(delta / ((double) gridMagnitude));
	if (n != 0) {
	    gridSizeX_ = delta / ((double) n);
	}
	else {
	    ownX = false;
	}
    }
    if (ownY) {
	delta = (double) (second.y() - first.y());
	n = (int) rint(delta / ((double) gridMagnitude));
	if (n != 0) {
	    gridSizeY_ = delta / ((double) n);
	}
	else {
	    ownY = false;
	}
    }
    if (!ownX && !ownY) {
	gridSizeX_ = gridSizeY_ = (double) gridMagnitude;
    }
    else if (!ownX) {
	gridSizeX_ = gridSizeY_;
    }
    else if (!ownY) {
	gridSizeY_ = gridSizeX_;
    }
}


Grid::~Grid()
{
}


QPoint Grid::closestGridPoint(QPoint point) const
{
    // offset + n * gridSize - point = min
    int x = (int) rint(((double) point.x() - offsetX_) / gridSizeX_);
    int y = (int) rint(((double) point.y() - offsetY_) / gridSizeY_);
    return QPoint((int) rint(offsetX_ + ((double) x) * gridSizeX_),
		  (int) rint(offsetY_ + ((double) y) * gridSizeY_));
}


QPoint Grid::move(QPoint point, int x, int y) const
{
    double dx = ((double) x) * gridSizeX_;
    double dy = ((double) y) * gridSizeY_;
    return closestGridPoint(QPoint(point.x() + ((int) dx),
				   point.y() + ((int) dy)));
}


QPoint Grid::move(QPoint point, LineDirection dir, int distance) const
{
    switch(dir) {
    case LEFT:
	return move(point, -distance, 0);
    case RIGHT:
	return move(point, distance, 0);
    case UP:
	return move(point, 0, -distance);
    case DOWN:
	return move(point, 0, distance);
    default:
	Q_ASSERT(false);
	return QPoint(0,0);
    }
}


void Grid::getGridDistance(QPoint start, QPoint end, int &x, int &y) const
{
    double dx = (double) end.x() - start.x();
    double dy = (double) end.y() - start.y();
    x = (int) rint(dx / gridSizeX_);
    y = (int) rint(dy / gridSizeY_);
}
