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
 * $Id: canvasview.cpp,v 1.1 2003/08/22 10:14:19 squig Exp $
 *
 *****************************************************************************/
#include "canvasview.h"

#include <qvariant.h>
#include <qpoint.h>
#include <qwmatrix.h>

/*****************************************************************************
 * Constructs the view.
 */
CanvasView::CanvasView(QCanvas *canvas, QWidget *parent, const char* name,
                       WFlags fl)
    : QCanvasView(canvas, parent, name, fl)
{
}

/*****************************************************************************
 * Destroys the view.
 */
CanvasView::~CanvasView()
{
    // no need to delete child widgets, Qt does it all for us
}

void CanvasView::contentsMousePressEvent(QMouseEvent* e)
{
    QPoint p = inverseWorldMatrix().map(e->pos());
    QCanvasItemList l = canvas()->collisions(p);
    for (QCanvasItemList::Iterator it = l.begin(); it != l.end(); ++it) {
        // first item is top item
        movingItem = *it;
        movingStartPoint = p;
        return;
    }
    movingItem = 0;
}

void CanvasView::contentsMouseMoveEvent(QMouseEvent* e)
{
    if (movingItem) {
        QPoint p = inverseWorldMatrix().map(e->pos());
        movingItem->moveBy(p.x() - movingStartPoint.x(),
                           p.y() - movingStartPoint.y());
        movingStartPoint = p;
        canvas()->update();
    }
}
