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
 * $Id: gridcanvas.cpp,v 1.2 2003/08/21 13:18:30 garbeam Exp $
 *
 *****************************************************************************/

#include "gridcanvas.h"

#include <qcanvas.h>
#include <qpainter.h>
#include <qpen.h>
#include <qrect.h>
#include <qsize.h>

// size of tiles
#define SPACING 20


GridCanvas::GridCanvas() {

    // create grid pixmap
    QRect r(0, 0, SPACING, SPACING);
    tile = new QPixmap(r.size());
    tile->fill(white);

    QPen pen(gray, 1);
    QPainter p(tile);
    p.setPen(pen);
    p.drawLine(SPACING / 2, (SPACING / 2) - 2, SPACING / 2, (SPACING / 2) + 3);
    p.drawLine((SPACING / 2) - 2, SPACING / 2, (SPACING / 2) + 3, SPACING / 2);
    p.end();

    connect(this, SIGNAL(resized()), this, SLOT(redrawGrid()));

    resize(1600, 1400);

}

void GridCanvas::redrawGrid() {

    int w = size().width();
    int h = size().height();

    setTiles(*tile, w, h, SPACING, SPACING);
}
