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
 * $Id: grid.cpp,v 1.2 2003/09/18 01:51:17 keulsn Exp $
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
    return QPoint((int) (0.5 + offsetX_ + ((double) x) * gridSizeX_),
		  (int) (0.5 + offsetY_ + ((double) y) * gridSizeY_));
}


QPoint Grid::move(QPoint point, int x, int y)
{
    double dx = ((double) x) * gridSizeX_;
    double dy = ((double) y) * gridSizeY_;
    return closestGridPoint(QPoint(point.x() + ((int) dx),
				   point.y() + ((int) dy)));
}


QPoint Grid::move(QPoint point, LineDirection dir, int distance)
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


void Grid::getGridDistance(QPoint start, QPoint end, int &x, int &y)
{
    double dx = (double) end.x() - start.x();
    double dy = (double) end.y() - start.y();
    x = (int) (0.5 + dx / gridSizeX_);
    y = (int) (0.5 + dy / gridSizeY_);
}
