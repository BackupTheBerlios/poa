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
 * $Id: canvasview.cpp,v 1.13 2003/08/27 21:12:45 vanto Exp $
 *
 *****************************************************************************/
#include "canvasview.h"

#include "abstractmodel.h"
#include "abstractview.h"
#include "cpumodel.h"
#include "project.h"
#include "mainwindow.h"
#include "modelfactory.h"

#include <qvariant.h>
#include <qapplication.h>
#include <qdom.h>
#include <qpoint.h>
#include <qwmatrix.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qstatusbar.h>

#include <typeinfo>

/*****************************************************************************
 * Constructs the view.
 */
CanvasView::CanvasView(Project *project, QCanvas *canvas, QWidget *parent,
                       const char* name, WFlags fl)
    : QCanvasView(canvas, parent, name, fl), project_(project),
      movingItem_(0)
{
    currentZ_ = 0;
    setAcceptDrops(TRUE);

    connect(project, SIGNAL(modelAdded(AbstractModel *, int, int)),
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
    if (e->button() == LeftButton) {
        if (!l.isEmpty()) {
            // first item is top item
            movingItem_ = l.first();
            movingStartPoint_ = p;
        }
        else {
            movingItem_ = 0;
        }
    }
    else if (e->button() == RightButton) {
        if (!l.isEmpty()) {
            QPopupMenu *menu = (dynamic_cast<AbstractView *>(l.first()))->popupMenu();
            if (menu) {
                menu->exec(e->pos());
            }
        }
    }
}

void CanvasView::contentsMouseMoveEvent(QMouseEvent* e)
{
    if (movingItem_) {
        QPoint p = inverseWorldMatrix().map(e->pos());
        movingItem_->moveBy(p.x() - movingStartPoint_.x(),
                            p.y() - movingStartPoint_.y());
        movingStartPoint_ = p;
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
            QValueList<AbstractModel *> l = ModelFactory::generate(doc);
            for (QValueList<AbstractModel *>::Iterator it = l.begin();
                 it != l.end(); ++it) {
                project_->add(*it, pos.x(), pos.y());
            }
        }
    }
}

void CanvasView::modelAdded(AbstractModel *item, int x, int y)
{
    ++currentZ_;
    QCanvasItemList l = item->createView(canvas());
    for (QCanvasItemList::Iterator it = l.begin(); it != l.end(); ++it) {
        (*it)->moveBy(x, y);
    (*it)->setZ(currentZ_);
        (*it)->show();
    }
    canvas()->update();
}

QPoint CanvasView::toCanvas(QPoint pos)
{
    return inverseWorldMatrix().map(viewportToContents(pos));
}
