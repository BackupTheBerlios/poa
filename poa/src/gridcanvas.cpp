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
 * $Id: gridcanvas.cpp,v 1.5 2003/08/22 15:30:38 garbeam Exp $
 *
 *****************************************************************************/

#include "gridcanvas.h"

#include <qcanvas.h>
#include <qpainter.h>
#include <qpen.h>
#include <qrect.h>
#include <qsize.h>

// space between grid crosses
#define GRID_SPACING 20

GridCanvas::GridCanvas() {

    // create grid pixmap
    QRect r(0, 0, 1000, 1000);
    gridTile= new QPixmap(r.size());
    gridTile->fill(white);

    QPen pen(gray, 1);
    QPainter p(gridTile);
    p.setPen(pen);

    for (int x = 0; x <= 1000; x += GRID_SPACING) {
        for (int y = 0; y <= 1000; y += GRID_SPACING) {

            p.drawLine(x + (GRID_SPACING / 2) /* x1 */,
                       y + ((GRID_SPACING / 2) - 2) /* y1 */,
                       x + (GRID_SPACING / 2) /* x2 */,
                       y + ((GRID_SPACING / 2) + 3) /* y2 */);
            p.drawLine(x + ((GRID_SPACING / 2) - 2) /* x1 */,
                       y + (GRID_SPACING / 2) /* y1 */,
                       x + ((GRID_SPACING / 2) + 3) /* x2 */,
                       y + (GRID_SPACING / 2) /* y2 */);
        }
    }
    p.end();

    connect(this, SIGNAL(resized()), this, SLOT(redrawGrid()));

    resize(1000, 1000);

}

void GridCanvas::redrawGrid() {

    setBackgroundPixmap(*gridTile);
}
