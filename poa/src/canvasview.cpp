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
 * $Id: canvasview.cpp,v 1.6 2003/08/22 15:27:43 garbeam Exp $
 *
 *****************************************************************************/
#include "canvasview.h"

#include "cpumodel.h"

#include <qvariant.h>
#include <qdom.h>
#include <qpoint.h>
#include <qwmatrix.h>
#include <qpainter.h>

/*****************************************************************************
 * Constructs the view.
 */
CanvasView::CanvasView(QCanvas *canvas, QWidget *parent, const char* name,
                       WFlags fl)
    : QCanvasView(canvas, parent, name, fl)
{
    setAcceptDrops(TRUE);
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

void CanvasView::dragEnterEvent(QDragEnterEvent *e)
{
    e->accept(e->provides("text/xml"));
}

void CanvasView::dropEvent(QDropEvent *e)
{
    QByteArray data = e->encodedData("text/xml");
    if (data) {
        QDomDocument *doc = new QDomDocument("cpu");
        if (doc->setContent(QString(data))) {
            // FIX: remove: create dummy items
            QPoint pos = viewportToContents(e->pos());
            QCanvasPolygonalItem *i
                = new QCanvasRectangle(pos.x(), pos.y(), 100, 100, canvas());
            i->setBrush(QColor(255, 255, 255));
            i->setPen(QPen(QColor(0, 0, 0), 2));
            i->show();
            canvas()->update();
        }
    }
}
