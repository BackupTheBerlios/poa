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
 * $Id: canvasview.cpp,v 1.16 2003/08/28 16:29:59 keulsn Exp $
 *
 *****************************************************************************/


#include "canvasview.h"

#include "abstractmodel.h"
#include "abstractview.h"
#include "blockview.h"
#include "cpumodel.h"
#include "poa.h"
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


CanvasView::CanvasView(Project *project, QCanvas *canvas, QWidget *parent,
                       const char* name, WFlags fl)
    : QCanvasView(canvas, parent, name, fl), project_(project),
      movingItem_(0)
{
    setAcceptDrops(TRUE);
}

CanvasView::~CanvasView()
{
    // no need to delete child widgets, Qt does it all for us
}

Project *CanvasView::project()
{
    return project_;
}

void CanvasView::contentsMousePressEvent(QMouseEvent *e)
{
    QPoint p = inverseWorldMatrix().map(e->pos());
    QCanvasItemList l = canvas()->collisions(p);
    if (e->button() == LeftButton) {
        if (!l.isEmpty()) {
            // first item is top item
	    QCanvasItem *topItem = l.first();
	    if (INSTANCEOF(topItem, BlockView)) {
		movingItem_ = topItem;
		movingStartPoint_ = p;
	    }
        }
    }
    else if (e->button() == RightButton) {
        if (!l.isEmpty()) {
	    AbstractView *item = dynamic_cast<AbstractView *>(l.first());
	    // item may be 0 if !INSTANCEOF(l.first(), AbstractView)
	    if (item != 0) {
		QPopupMenu *menu = item->popupMenu();
		if (menu) {
		    menu->exec(e->pos());
		}
	    }
        }
    }
}

void CanvasView::contentsMouseReleaseEvent(QMouseEvent *)
{
    movingItem_ = 0;
}

void CanvasView::contentsMouseMoveEvent(QMouseEvent *e)
{
    if (movingItem_ != 0) {
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
                project_->add(*it);
                ((GridCanvas *)canvas())->addView(*it, pos.x(), pos.y());
            }
        }
    }
}


QPoint CanvasView::toCanvas(QPoint pos)
{
    return inverseWorldMatrix().map(viewportToContents(pos));
}
