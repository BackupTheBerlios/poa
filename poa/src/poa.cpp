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
 * $Id: poa.cpp,v 1.1 2003/12/03 14:43:54 keulsn Exp $
 *
 *****************************************************************************/


#include "poa.h"


QString image(QPoint p)
{
    return "(" + QString::number(p.x()) + ", " + QString::number(p.y()) + ")";
}


LineDirection operator-(LineDirection dir)
{
    switch (dir) {
    case LEFT:
        return RIGHT;
    case RIGHT:
        return LEFT;
    case UP:
        return DOWN;
    case DOWN:
        return UP;
    case UNKNOWN:
        return UNKNOWN;
    default:
        Q_ASSERT(false);
        return UNKNOWN;
    }
}

int distInDir(LineDirection dir, int x, int y)
{
    switch (dir) {
    case LEFT:
        return -x;
    case RIGHT:
        return x;
    case UP:
        return -y;
    case DOWN:
        return y;
    default:
        Q_ASSERT(false);
        return 0;
    }
}

LineDirection alternateDir(LineDirection dir, int x, int y)
{
    switch (dir) {
    case LEFT:
    case RIGHT:
        if (y > 0) {
            return DOWN;
        }
        else {
            return UP;
        }
    case UP:
    case DOWN:
        if (x > 0) {
            return RIGHT;
        }
        else {
            return LEFT;
        }
    default:
        Q_ASSERT(false);
        return UNKNOWN;
    }
}


bool isRightAngle(LineDirection first, LineDirection second)
{
    switch (first) {
    case LEFT:
    case RIGHT:
        return second == UP || second == DOWN;
    case UP:
    case DOWN:
        return second == LEFT || second == RIGHT;
    case UNKNOWN:
        return false;
    default:
        Q_ASSERT(false);
        return false;
    }
}

bool isTurn(LineDirection first, LineDirection second)
{
  Q_ASSERT(first != UNKNOWN && second != UNKNOWN);
  return first == LEFT && second == RIGHT
    || first == RIGHT && second == LEFT
    || first == UP && second == DOWN
    || first == DOWN && second == UP;
}

LineDirection turnLeft(LineDirection dir)
{
    switch (dir) {
    case LEFT:
        return DOWN;
    case DOWN:
        return RIGHT;
    case RIGHT:
        return UP;
    case UP:
        return LEFT;
    default:
        Q_ASSERT(false);
        return UNKNOWN;
    }
}

LineDirection turnRight(LineDirection dir)
{
    return -turnLeft(dir);
}

QString image(LineDirection dir)
{
    switch(dir) {
    case LEFT:
	return QString("left");
    case RIGHT:
	return QString("right");
    case UP:
	return QString("up");
    case DOWN:
	return QString("down");
    }
    return 0;
}
