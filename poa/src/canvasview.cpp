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
 * $Id: canvasview.cpp,v 1.23 2003/09/07 19:07:46 squig Exp $
 *
 *****************************************************************************/

#include "canvasview.h"

#include "abstractmodel.h"
#include "abstractview.h"
#include "blockview.h"
#include "canvasviewaction.h"
#include "connectormodel.h"
#include "cpumodel.h"
#include "pinmodel.h"
#include "pinview.h"
#include "poa.h"
#include "project.h"
#include "mainwindow.h"
#include "modelfactory.h"
#include "settings.h"

#include <qvariant.h>
#include <qapplication.h>
#include <qcursor.h>
#include <qdom.h>
#include <qpoint.h>
#include <qwmatrix.h>
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qstatusbar.h>
#include <qtooltip.h>

const int AUTOSCROLL_MARGIN = 16;

CanvasView::CanvasView(Project *project, GridCanvas *canvas, QWidget *parent,
                       const char* name, WFlags fl)
    : QCanvasView(canvas, parent, name, fl), project_(project), action_(0)
{
    setAcceptDrops(true);
    setDragAutoScroll(true);
}

CanvasView::~CanvasView()
{
    // no need to delete child widgets, Qt does it all for us
}

GridCanvas *CanvasView::gridCanvas()
{
    return static_cast<GridCanvas *>(canvas());
}

Project *CanvasView::project()
{
    return project_;
}

void CanvasView::contentsMousePressEvent(QMouseEvent *e)
{
    if (action_) {
        action_->mousePressEvent(e);
        return;
    }

    QPoint p = inverseWorldMatrix().map(e->pos());
    QCanvasItemList l = canvas()->collisions(p);
    if (e->button() == LeftButton) {
        if (!l.isEmpty()) {
            // first item is top item
            QCanvasItem *topItem = l.first();

            // select item, we only support single selection for now
            deselectAll();
            topItem->setSelected(true);
            emit(selectionChanged(true));

            // notify item that is has been clicked
            AbstractView *item = dynamic_cast<AbstractView*>(topItem);
            if (item != 0) {
                item->mousePressEvent(this, e);
            }

            canvas()->update();
        }
        else {
            // nirvana click
            deselectAll();
            canvas()->update();
        }
    }
    else if (e->button() == RightButton) {
        if (!l.isEmpty()) {
            // first item is top item
            QCanvasItem *topItem = l.first();

            // select item
            deselectAll();
            topItem->setSelected(true);
            emit(selectionChanged(true));
            canvas()->update();

            AbstractView *item = dynamic_cast<AbstractView *>(topItem);
            if (item != 0) {
                // show popup menu if available
                QPopupMenu *menu = item->popupMenu();
                if (menu != 0) {
                    menu->exec(mapToGlobal(e->pos()));
                }
            }
        }
        else {
            // show background popup menu
        }
    }
}

void CanvasView::contentsMouseReleaseEvent(QMouseEvent *e)
{
    if (action_) {
        action_->mouseReleaseEvent(e);
    }
}

void CanvasView::contentsMouseMoveEvent(QMouseEvent *e)
{
    QPoint pos = toCanvas(e->pos());
    ((MainWindow *)qApp->mainWidget())->statusBar()->message
        (QString::number(pos.x()) + ":" + QString::number(pos.y()));

    doAutoScroll();

    if (action_) {
        action_->mouseMoveEvent(e);
    }
}

void CanvasView::deselectAll()
{
    QCanvasItemList allItems = canvas()->allItems();
    for (QCanvasItemList::iterator current = allItems.begin();
         current != allItems.end(); ++current) {

        (*current)->setSelected(false);
    }
    emit(selectionChanged(false));
}

void CanvasView::doAutoScroll()
{
    QPoint p = mapFromGlobal(QCursor::pos());

    int dx = 0, dy = 0;
    if (p.y() < AUTOSCROLL_MARGIN) {
        dy = -1;
    }
    else if (p.y() > visibleHeight() - AUTOSCROLL_MARGIN) {
        dy = +1;
    }
    if ( p.x() < AUTOSCROLL_MARGIN ) {
        dx = -1;
    } else if ( p.x() > visibleWidth() - AUTOSCROLL_MARGIN) {
        dx = +1;
    }
    if ( dx || dy ) {
        scrollBy(dx,dy);
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
        QDomDocument doc;
        if (doc.setContent(QString(data))) {
            QPoint pos = toCanvas(e->pos());
            QValueList<AbstractModel *> l = ModelFactory::generate(doc);
            for (QValueList<AbstractModel *>::Iterator it = l.begin();
                 it != l.end(); ++it) {
                project()->addBlock(*it);
                gridCanvas()->addView(*it, pos.x(), pos.y());
            }
        }
    }
}

QCanvasItemList CanvasView::selectedItems()
{
    QCanvasItemList selectedItems;
    QCanvasItemList allItems = canvas()->allItems();
    for (QCanvasItemList::iterator current = allItems.begin();
         current != allItems.end(); ++current) {

        if ((*current)->isSelected()) {
            selectedItems.append(*current);
        }
    }
    return selectedItems;
}

void CanvasView::setAction(CanvasViewAction *action)
{
    if (action_) {
        action_->cancel();
    }
    action_ = action;

    // FIX: this call should go somewhere else
    canvas()->update();
}

QPoint CanvasView::toCanvas(QPoint pos)
{
    return inverseWorldMatrix().map(viewportToContents(pos));
}
