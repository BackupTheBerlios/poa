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
 * $Id: gridcanvas.cpp,v 1.13 2003/08/29 17:59:38 vanto Exp $
 *
 *****************************************************************************/

#include "gridcanvas.h"

#include "project.h"
#include "settings.h"

#include <qcanvas.h>
#include <qpainter.h>
#include <qpen.h>
#include <qrect.h>
#include <qsize.h>

GridCanvas::GridCanvas(QString name)
{
    setName(name);
    currentZ_ = 0;
    setGridSize(Settings::instance()->gridSize());
    setDoubleBuffering(TRUE);

    connect(Settings::instance(), SIGNAL(gridSizeChanged(int)),
            this, SLOT(setGridSize(int)));
}

void GridCanvas::addViewAt(AbstractModel *item, int x, int y)
{
    ++currentZ_;
    QCanvasItemList l = item->createView(this);
    for (QCanvasItemList::Iterator it = l.begin(); it != l.end(); ++it) {
        (*it)->moveBy(x, y);
        (*it)->setZ(currentZ_);
        (*it)->show();
    }
    update();
}

void GridCanvas::addView(AbstractModel *item) {
    ++currentZ_;
    QCanvasItemList l = item->createView(this);
    for (QCanvasItemList::Iterator it = l.begin(); it != l.end(); ++it) {
        (*it)->setZ(currentZ_);
        (*it)->show();
    }
    update();
}


void GridCanvas::setGridSize(int gridSize)
{
    int stretch = 10;
    // create grid pixmap
    QRect r(0, 0, stretch *  gridSize, stretch * gridSize);
    QPixmap gridTile(r.size());
    gridTile.fill(white);

    QPainter p(&gridTile);
    p.setPen(QPen(gray, 1));

    for (int i = 1; i <= stretch; i++) {
        p.drawLine(0,
                   i * gridSize - 2,
                   r.size().width(),
                   i * gridSize - 2);
        p.drawLine(i * gridSize - 2,
                   0,
                   i * gridSize - 2,
                   r.size().height());
    }
//             p.drawLine(x + (gridSize / 2) /* x1 */,
//                        y + ((gridSize / 2) - 2) /* y1 */,
//                        x + (gridSize / 2) /* x2 */,
//                        y + ((gridSize / 2) + 3) /* y2 */);
//             p.drawLine(x + ((gridSize / 2) - 2) /* x1 */,
//                        y + (gridSize / 2) /* y1 */,
//                        x + ((gridSize / 2) + 3) /* x2 */,
//                        y + (gridSize / 2) /* y2 */);
    p.end();

    setBackgroundPixmap(gridTile);
}
