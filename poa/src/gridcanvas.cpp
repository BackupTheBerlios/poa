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
 * $Id: gridcanvas.cpp,v 1.1 2003/08/21 10:46:37 garbeam Exp $
 *
 *****************************************************************************/

#include "gridcanvas.h"

#include <qcanvas.h>
#include <qpixmap.h>
#include <stdio.h>


// spacing pixels
#define SPACING 10

// a point consists of two black pixels in each direction
static const char* const grid_point_data[] = { 
    "10 10 3 1",
    "t c None",
    ". c #ffffff",
    "# c #000000",
    "..........",
    "..........",
    "..........",
    "..........",
    "....##....",
    "....##....",
    "..........",
    "..........",
    "..........",
    ".........."};


GridCanvas::GridCanvas(int h, int w) {
    QPixmap gridPoint((const char**) grid_point_data);
    //this->setBackgroundColor(QColor(y
    this->resize(h, w);
    this->setTiles(gridPoint, h, w, 10, 10);
//    this->setBackgroundPixmap(gridPoint);
};

//void GridCanvas::drawBackground(QPainter &painter, const QRect &clip)
//{
//    painter.setPen(black);
//    painter.setBrush(QBrush(Qt::CrossPattern));
//    // simply draws some points in a square grid
//    for (int x = 0; x <= /*clip.width()*/1000; x += SPACING) {
//        for (int y = 0; y <= /*clip.height()*/1000; y += SPACING) {
//            painter.drawPoint(x, y);
//        }
//    }
//}
//
