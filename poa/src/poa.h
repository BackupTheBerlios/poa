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
 * $Id: poa.h,v 1.17 2004/01/29 13:33:48 vanto Exp $
 *
 *****************************************************************************/

#ifndef POA_POA_H
#define POA_POA_H

#include <qrect.h>
#include <qpoint.h>

/*
 * Macro definitions
 */
#define INSTANCEOF(ptr,type) (((ptr) != 0) && (dynamic_cast<type*>(ptr) != 0))

#ifndef rint
#define rint(a) (floor((a)+0.5))
#endif

static const unsigned MAX_RECENT_PROJECTS = 5;
static const unsigned WIDGET_SPACING = 5;

/**
 * Returns a string representation of <code>p</code>
 */
QString image(QPoint p);

QString image(QRect r);


/** Direction of a line to be drawn on a canvas. */
enum LineDirection {UNKNOWN, UP, DOWN, LEFT, RIGHT};

/**
 * Returns the reverse direction.
 * LEFT -> RIGHT, UP -> DOWN, vice versa
 */
LineDirection reverse(LineDirection dir);

/**
 *
 */
int distInDir(LineDirection dir, int x, int y);


LineDirection alternateDir(LineDirection dir, int x, int y);

/**
 * Returns true if the given directions are orthogonal.
 */
bool isRightAngle(LineDirection first, LineDirection second);

/**
 * Returns true if the first direction is the opposite of the
 * second direction.
 */
bool isTurn(LineDirection first, LineDirection second);

/**
 * Returns the direction which results out of a left turn from
 * the given direction.
 */
LineDirection turnLeft(LineDirection dir);

/**
 * Returns the direction which results out of a right turn from
 * the given direction.
 */
LineDirection turnRight(LineDirection dir);

/**
 * Returns the direction as a lowercase string.
 */
QString image(LineDirection dir);

/**
 * Transforms the given duration in nanoseconds to senseful units.
 */
QString formatTimeProperly(int ns);

#endif // POA_POA_H
