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
 * $Id: gridcanvas.cpp,v 1.14 2003/09/04 14:33:32 keulsn Exp $
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

int coordToGridHigher(int c, int gridSize)
{
    int nextC = c / gridSize;
    if ((c > 0) && ((c % gridSize) != 0)) {
	++nextC;
    }
    return nextC * gridSize;
}

int coordToGridLower(int c, int gridSize)
{
    int prevC = c / gridSize;
    if ((c < 0) && ((c % gridSize) != 0)) {
	--prevC;
    }
    return prevC * gridSize;
}

void GridCanvas::drawBackground(QPainter &painter, const QRect &clip)
{
    // clear background
    painter.fillRect(clip, QBrush (QColor("white")));

    int gridSize = Settings::instance()->gridSize();

    // draw grid
    painter.setPen(QPen(gray));
    int minX = coordToGridHigher(clip.left(), gridSize);
    int minY = coordToGridHigher(clip.top(), gridSize);
    int maxX = coordToGridLower(clip.right(), gridSize);
    int maxY = coordToGridLower(clip.bottom(), gridSize);

    // for some reason qt does not draw a line onto the pixels specified.
    // therefore we must subtract 1 at top and left coodinate and add 1 at
    // bottom and right coordinate.

    for (int x = minX; x <= maxX; x += gridSize) {
	painter.drawLine(x, clip.top() - 1, x, clip.bottom() + 1);
    }
    
    for (int y = minY; y <= maxY; y += gridSize) {
	painter.drawLine(clip.left() - 1, y, clip.right() + 1, y);
    }
}

void GridCanvas::setGridSize(int)
{
    setAllChanged();
    update();
}
