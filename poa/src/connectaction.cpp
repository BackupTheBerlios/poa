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
 * $Id: connectaction.cpp,v 1.1 2003/09/07 19:07:46 squig Exp $
 *
 *****************************************************************************/

#include "connectaction.h"

#include "blockmodel.h"
#include "canvasview.h"
#include "connectormodel.h"
#include "gridcanvas.h"
#include "pinmodel.h"
#include "pinview.h"
#include "project.h"
#include "settings.h"

#include <qcanvas.h>
#include <qcolor.h>
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
}

ConnectAction::~ConnectAction()
{
    view()->setMouseTracking(savedHasMouseTracking_);
}

void ConnectAction::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p = view()->inverseWorldMatrix().map(e->pos());
    line_.setPoints(line_.startPoint().x(), line_.startPoint().y(),
                    p.x(), p.y());
    view()->canvas()->update();
}

void ConnectAction::mouseReleaseEvent(QMouseEvent *e)
{
    QPoint p = view()->inverseWorldMatrix().map(e->pos());
    QCanvasItemList l = view()->canvas()->collisions(p);

    if (!l.isEmpty()) {
        QCanvasItem *topItem = l.first();
        PinView *target = dynamic_cast<PinView *>(topItem);
        if (target != 0 && target != source_) {
            // target is a PinView indeed
            ConnectorModel *cm
                = new ConnectorModel(source_->pinModel(), target->pinModel());
            // add connector to project
            view()->project()->addConnector(cm);
            view()->project()->createViews(cm);

            qWarning("Connect: "
                     + QString(source_->pinModel()->parent()->name())
                     + ":" + QString(source_->pinModel()->name()) + "\n"
                     + "to " + QString(target->pinModel()->parent()->name())
                     + ":" + QString(target->pinModel()->name()));
            view()->deselectAll();
        }
    }

    view()->setAction(0);
}
