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
 * $Id: pinview.cpp,v 1.10 2003/09/09 14:04:44 vanto Exp $
 *
 *****************************************************************************/


#include "pinview.h"

#include "blockview.h"
#include "canvasview.h"
#include "connectaction.h"
#include "pinmodel.h"
#include "settings.h"

#include <qevent.h>

PinView::PinView(PinModel *model, BlockView *block,
                 PinView::DockPosition dockPosition)
    : QCanvasRectangle(block->canvas())
{
    model_ = model;
    dockPosition_ = dockPosition;
    setZ(block->z());
    setBrush(QBrush(SolidPattern));
    switch (dockPosition) {
    case PinView::PIN_LEFT:
    case PinView::PIN_RIGHT:
        setSize(10, 2);
        break;
    case PinView::PIN_TOP:
    case PinView::PIN_BOTTOM:
        setSize(2, 10);
        break;
    }
}


PinView::~PinView()
{
}

AbstractModel *PinView::model()
{
    return model_;
}

PinView::DockPosition PinView::dockPosition()
{
    return dockPosition_;
}

QPoint PinView::connectorPoint()
{
    QRect r = rect();
    switch (dockPosition_) {
    case PIN_TOP:
        return QPoint(r.center().x(), r.top() - 1);
        break;
    case PIN_LEFT:
        return QPoint(r.left() - 1, r.center().y());
        break;
    case PIN_BOTTOM:
        return QPoint(r.center().x(), r.y() + 1);
        break;
    case PIN_RIGHT:
        return QPoint(r.right() + 1, r.center().y());
        break;
    default:
        Q_ASSERT(false);
        return QPoint(0, 0);
    }
}

void PinView::mousePressEvent(CanvasView *view, QMouseEvent *e)
{
    view->setAction(new ConnectAction(view, e, this));
}

PinModel *PinView::pinModel()
{
    return model_;
}

void PinView::setActive(bool yes)
{
    QCanvasRectangle::setActive(yes);
    updateProperties();
}

void PinView::setSelected(bool yes)
{
    QCanvasRectangle::setSelected(yes);
    updateProperties();
}

void PinView::updateProperties()
{
    if (isActive()) {
        setPen(Settings::instance()->activatedColor());
        setBrush(QBrush(Settings::instance()->activatedColor()));
    }
    else if (isSelected()) {
        setPen(Settings::instance()->selectedColor());
        setBrush(QBrush(Settings::instance()->selectedColor()));
    }
    else {
        setPen(Settings::instance()->defaultColor());
        setBrush(QBrush(Settings::instance()->defaultColor()));
    }
}
