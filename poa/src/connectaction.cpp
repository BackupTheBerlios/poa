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
 * $Id: connectaction.cpp,v 1.15 2004/02/05 14:00:23 papier Exp $
 *
 *****************************************************************************/

#include "connectaction.h"

#include "blockmodel.h"
#include "canvasview.h"
#include "gridcanvas.h"
#include "pinmodel.h"
#include "pinview.h"
#include "poa.h"
#include "project.h"
#include "settings.h"

#include <qapplication.h>
#include <qcanvas.h>
#include <qcolor.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <qwmatrix.h>

ConnectAction::ConnectAction(CanvasView *view, QMouseEvent *e,
                             PinView *source)
    : CanvasViewAction(view), source_(source), line_(view->canvas())
{
    savedHasMouseTracking_ = view->hasMouseTracking();
    view->setMouseTracking(true);

    QPoint p = view->inverseWorldMatrix().map(e->pos());
    line_.setPoints(p.x(), p.y(), p.x(), p.y());
    line_.setPen(QPen(Qt::black, 1, Qt::DashLine));
    line_.show();

    // mark all pins that are connectable
    QCanvasItemList allItems = view->canvas()->allItems();
    for (QCanvasItemList::iterator it = allItems.begin();
         it != allItems.end(); ++it) {
        if ((*it)->rtti() == PinView::RTTI
            && *it != source
            && dynamic_cast<PinView *>(*it)->parent() != source->parent()) {

            PinView *pv = (PinView *)(*it);
            pv->setActive(source->model()->isConnectable(pv->model()));
        }
    }
}

ConnectAction::~ConnectAction()
{
    view()->setMouseTracking(savedHasMouseTracking_);

    // unmark all items
    QCanvasItemList allItems = view()->canvas()->allItems();
    for (QCanvasItemList::iterator it = allItems.begin();
         it != allItems.end(); ++it) {
        (*it)->setActive(false);
    }
}

void ConnectAction::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p = view()->inverseWorldMatrix().map(e->pos());

    QCanvasItem *item = activeItemAt(p);
    if (item != 0) {
        Q_ASSERT(item->rtti() == PinView::RTTI);
        QPoint target = dynamic_cast<PinView*>(item)->connectorPoint();
        line_.setPoints(line_.startPoint().x(), line_.startPoint().y(),
                        target.x(), target.y());
    }
    else {
            // no active item is close to current position
            line_.setPoints(line_.startPoint().x(), line_.startPoint().y(),
                            p.x(), p.y());
    }
    view()->canvas()->update();
}

void ConnectAction::mouseReleaseEvent(QMouseEvent *e)
{
    QPoint p = view()->inverseWorldMatrix().map(e->pos());
    QCanvasItem *item = activeItemAt(p);
    if (item != 0) {
        PinView *target = dynamic_cast<PinView *>(item);
        if (target != 0 && target != source_) {
            // target is a PinView indeed
            source_->model()->attach(target->model());
            // add connector views
            view()->project()->createConnectorViews(source_, target);

            view()->deselectAll();
            line_.setVisible(false);
            view()->canvas()->update();


            int sourceBits = source_->model()->bits();
            int targetBits = target->model()->bits();
            if (sourceBits != targetBits) {
                int ans = QMessageBox::information
                    (0, "POA",
                     QString(qApp->translate("connectaction",
					     "The pins have different widths\n"
					     "Do you want to change the width?")),
                     QString(qApp->translate("connectaction",
					     "Set %1 bits to %2"))
                     .arg(source_->model()->name()).arg(targetBits),
                     QString(qApp->translate("connectaction",
					     "Set %1 bits to %2"))
                     .arg(target->model()->name()).arg(sourceBits),
                     qApp->translate("connectaction","Cancel"),
                     0,  // Default == button 0
                     2); // Escape  == button 2
                switch(ans) {
                case 0: // Adjust source
                    source_->model()->setBits(targetBits);
                    source_->model()->updatePerformed();
                    break;
                case 1: // Adjust target
                    target->model()->setBits(sourceBits);
                    target->model()->updatePerformed();
                    break;
                default: // Cancel clicked or Alt+C pressed or Escape pressed
                    break;
                }
            }
        }
    }

    view()->setAction(0);
}

QCanvasItem *ConnectAction::activeItem(QCanvasItemList items)
{
    for (QCanvasItemList::iterator it = items.begin();
         it != items.end(); ++it) {
        if ((*it)->isActive()) {
            return (*it);
        }
    }
    return 0;
}

QCanvasItem *ConnectAction::activeItemAt(QPoint p)
{
    QCanvasItem *item;

    // try exact collision first
    if ((item = activeItem(view()->canvas()->collisions(p))) == 0) {
        // try bounding rect
        QRect hotspots(p.x() - 2, p.y() - 2, 5, 5);
        item = activeItem(view()->canvas()->collisions(hotspots));
    }
    return item;
}
