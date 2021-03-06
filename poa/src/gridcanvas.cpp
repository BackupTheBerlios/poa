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
 * $Id: gridcanvas.cpp,v 1.38 2004/02/11 13:01:41 squig Exp $
 *
 *****************************************************************************/

#include "connectorviewlist.h"
#include "dijkstrarouter.h"
#include "directrouter.h"
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
    currentZ_ = 0;
    router_ = new DirectRouter();

    colormanager_ = new ColorManager(this, Palette::createLightPalette());

    setName(name);
    setDoubleBuffering(TRUE);

    connect(Settings::instance(), SIGNAL(gridSizeChanged(int)),
            this, SLOT(updateAll()));
    connect(Settings::instance(), SIGNAL(showGridChanged(bool)),
            this, SLOT(updateAll()));

    colormanager_->setZ(incZ());
    colormanager_->show();
}

GridCanvas::~GridCanvas()
{
    delete router_;
    delete colormanager_;
}

void GridCanvas::addConnectorView(ConnectorViewList *viewList)
{
    route(viewList);

    QCanvasItemList segments = viewList->allSegments();
    for (QCanvasItemList::Iterator it = segments.begin();
     it != segments.end();
     ++it) {

        (*it)->setZ(0);
        (*it)->show();
    }
    update();
}

void GridCanvas::addView(AbstractModel *item, int x, int y)
{
    double z = incZ();
    QCanvasItemList l = item->createView(this);
    for (QCanvasItemList::Iterator it = l.begin(); it != l.end(); ++it) {
        if (x || y) {
            (*it)->moveBy(x, y);
        }
        (*it)->setZ(z);
        (*it)->show();
        ensureVisibility(*it);
    }
    // FIX: what the fuck do we need that for? View objects should take care
    // of the updating themselfs
    //connect(item, SIGNAL(updated()), this, SLOT(updateAll()));
    update();
}

void GridCanvas::addView(QCanvasItem *item, int x, int y)
{
    double z = incZ();
    item->moveBy(x, y);
    item->setZ(z);
    item->show();
    ensureVisibility(item);
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
    painter.fillRect(clip, QBrush (Qt::white));

    if (Settings::instance()->showGrid()) {
        int gridSize = Settings::instance()->gridSize();

        // draw grid
        painter.setPen(QPen(Qt::lightGray));

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
}

void GridCanvas::ensureVisibility(QCanvasItem *item)
{
    QRect rect = item->boundingRect();
    int newWidth = QMAX((int)rect.x() + rect.width(), width());
    int newHeight = QMAX((int)rect.y() + rect.height(), height());
    if (newWidth > width() || newHeight > height()) {
        resize(newWidth, newHeight);
    }
}

void GridCanvas::route(ConnectorViewList* connector)
{
    router_->route(connector);
}

double GridCanvas::incZ()
{
    return ++currentZ_;
}

QPoint GridCanvas::toGrid(QPoint p)
{
    int gridSize = Settings::instance()->gridSize();
    int x = coordToGridLower(p.x(), gridSize);
    if (p.x() - x > gridSize / 2) {
        x += gridSize;
    }
    int y = coordToGridLower(p.y(), gridSize);
    if (p.y() - y > gridSize / 2) {
        y += gridSize;
    }
    return QPoint(x, y);
}

void GridCanvas::updateAll()
{
    setAllChanged();
    update();
}

ColorManager *GridCanvas::colorManager()
{
    return colormanager_;
}
