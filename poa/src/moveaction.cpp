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
 * $Id: moveaction.cpp,v 1.5 2004/01/22 22:46:51 squig Exp $
 *
 *****************************************************************************/

#include "moveaction.h"

#include "canvasview.h"
#include "gridcanvas.h"
#include "moveable.h"
#include "settings.h"

//#include <qcanvas.h>
#include <qwmatrix.h>

MoveAction::MoveAction(CanvasView *view, QMouseEvent *e, Moveable *item)
    : CanvasViewAction(view), item_(item)
{
    startPoint_ = view->inverseWorldMatrix().map(e->pos());
}

void MoveAction::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p = view()->inverseWorldMatrix().map(e->pos());
    int dx = 0;
    int dy = 0;

    if (Settings::instance()->snapToGrid() &&
        !(e->stateAfter() & Qt::ControlButton)) {
        QPoint oldPos = QPoint((int)item_->item()->x(), (int)item_->item()->y());

        // move to closest grid coordinate
        QPoint newPos = view()->gridCanvas()->toGrid(oldPos + p - startPoint_);

        dx = newPos.x() - oldPos.x();
        dy = newPos.y() - oldPos.y();
    }
    else {
        dx = p.x() - startPoint_.x();
        dy = p.y() - startPoint_.y();
    }

    // make sure item is not moved outside of canvas
    if (item_->item()->x() + dx < 0) {
        dx = (int)item_->item()->x();
    }
    if (item_->item()->y() + dy < 0) {
        dy = (int)item_->item()->y();
    }

    if (dx != 0 || dy != 0) {
        QPoint moved = item_->dragBy(dx, dy);
        startPoint_ += moved;

        view()->gridCanvas()->ensureVisibility(item_->item());
        view()->canvas()->update();
    }
}

void MoveAction::mouseReleaseEvent(QMouseEvent *)
{
    view()->setAction(0);
}
