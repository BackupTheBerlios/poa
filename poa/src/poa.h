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
 * $Id: poa.h,v 1.10 2003/12/03 18:22:47 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_POA_H
#define POA_POA_H

#include <qpoint.h>

/*
 * Macro definitions
 */
#define INSTANCEOF(ptr,class) (((ptr) != 0) && (dynamic_cast<class*>((ptr)) != 0))

static const unsigned MAX_RECENT_PROJECTS = 5;
static const unsigned WIDGET_SPACING = 5;


/**
 * Returns a string representation of <code>p</code>
 */
QString image(QPoint p);


/** Direction of a line to be drawn on a canvas. */
enum LineDirection {UNKNOWN, UP, DOWN, LEFT, RIGHT};

//    static LineDirection reverse(LineDirection dir);
int distInDir(LineDirection dir, int x, int y);
LineDirection alternateDir(LineDirection dir, int x, int y);
bool isRightAngle(LineDirection first, LineDirection second);
bool isTurn(LineDirection first, LineDirection second);
LineDirection turnLeft(LineDirection dir);
LineDirection turnRight(LineDirection dir);
QString image(LineDirection dir);
LineDirection operator-(LineDirection dir);


#endif // POA_POA_H
