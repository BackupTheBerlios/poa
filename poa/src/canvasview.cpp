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
 * $Id: canvasview.cpp,v 1.8 2003/08/22 17:39:04 squig Exp $
 *
 *****************************************************************************/
#include "canvasview.h"

#include "abstractmodel.h"
#include "cpumodel.h"
#include "document.h"
#include "mainwindow.h"
#include "modelfactory.h"

#include <qvariant.h>
#include <qapplication.h>
#include <qdom.h>
#include <qpoint.h>
#include <qwmatrix.h>
#include <qpainter.h>
#include <qstatusbar.h>

/*****************************************************************************
 * Constructs the view.
 */
CanvasView::CanvasView(Document *document, QCanvas *canvas, QWidget *parent,
                       const char* name, WFlags fl)
    : QCanvasView(canvas, parent, name, fl), document_(document)
{
    setAcceptDrops(TRUE);

    connect(document, SIGNAL(modelAdded(AbstractModel *, int, int)),
            this, SLOT(modelAdded(AbstractModel *, int, int)));
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

    QPoint pos = toCanvas(e->pos());
    ((MainWindow *)qApp->mainWidget())->statusBar()->message
        (QString::number(pos.x()) + ":" + QString::number(pos.y()));
}

void CanvasView::dragEnterEvent(QDragEnterEvent *e)
{
    e->accept(e->provides("text/xml"));
}

void CanvasView::dropEvent(QDropEvent *e)
{
    QByteArray data = e->encodedData("text/xml");
    if (data) {
        QDomDocument doc;
        if (doc.setContent(QString(data))) {
            QPoint pos = toCanvas(e->pos());
            ModelFactory::generate(doc, document_, pos.x(), pos.y());
        }
    }
}

void CanvasView::modelAdded(AbstractModel *item, int x, int y)
{
    QCanvasItem *i = item->createView(canvas());
    i->moveBy(x, y);
    i->show();

    canvas()->update();
}

QPoint CanvasView::toCanvas(QPoint pos)
{
    return inverseWorldMatrix().map(viewportToContents(pos));
}
