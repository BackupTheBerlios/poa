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
 * $Id: pinview.cpp,v 1.31 2004/01/13 00:28:29 squig Exp $
 *
 *****************************************************************************/


#include "pinview.h"

#include "blockview.h"
#include "canvasview.h"
#include "connectaction.h"
#include "connectorviewlist.h"
#include "pinmodel.h"
#include "settings.h"

#include <qevent.h>

const int PinView::RTTI = 1001;


PinView::PinView(PinModel *model, BlockView *block,
                 PinView::DockPosition dockPosition)
    : QCanvasRectangle(block->canvas())
{
    model_ = model;
    block_ = block;
    connector_ = 0;
    connect(model_, SIGNAL(deleted()), this, SLOT(deleteView()));
    connect(model_, SIGNAL(detached()), this, SLOT(deleteConnection()));

    dockPosition_ = dockPosition;
    setZ(block->z());
    setBrush(QBrush(QObject::SolidPattern));
    switch (dockPosition) {
    case PinView::PIN_LEFT:
    case PinView::PIN_RIGHT:
        setSize(10, 3);
        break;
    case PinView::PIN_TOP:
    case PinView::PIN_BOTTOM:
        setSize(3, 10);
        break;
    }
}

PinView::~PinView()
{
    block_->deletePinView(this);
}

int PinView::rtti() const
{
    return PinView::RTTI;
}

ConnectorViewList *PinView::connector()
{
    return connector_;
}

PinView::DockPosition PinView::dockPosition()
{
    return dockPosition_;
}

void PinView::moveBy(double dx, double dy) {
    QCanvasRectangle::moveBy(dx, dy);
    if (connector() != 0) {
        // FIX: someone needs to be notified, the connection needs
        // some rerouting
        //connector()->pinMoved(this);
    }
}

QPoint PinView::connectorPoint()
{
    QRect r = rect();
    switch (dockPosition_) {
    case PIN_TOP:
        return QPoint(r.center().x(), r.top());
        break;
    case PIN_LEFT:
        return QPoint(r.left(), r.center().y());
        break;
    case PIN_BOTTOM:
        return QPoint(r.center().x(), r.bottom());
        break;
    case PIN_RIGHT:
        return QPoint(r.right(), r.center().y());
        break;
    default:
        Q_ASSERT(false);
        return QPoint(0, 0);
    }
}

LineDirection PinView::connectorSourceDir()
{
    return reverse(connectorTargetDir());
}

LineDirection PinView::connectorTargetDir()
{
    LineDirection dir;
    switch (dockPosition_) {
    case PIN_TOP:
        dir = DOWN;
        break;
    case PIN_LEFT:
        dir = RIGHT;
        break;
    case PIN_RIGHT:
        dir = LEFT;
        break;
    case PIN_BOTTOM:
        dir = UP;
        break;
    default:
        Q_ASSERT(false);
        // make compiler happy
        return UP;
    }
    return dir;
}

bool PinView::isConnected()
{
    return model()->connected() != 0;
}

PinModel *PinView::model()
{
    return model_;
}

void PinView::setActive(bool yes)
{
    QCanvasRectangle::setActive(yes);
    updateProperties();
}

void PinView::setConnector(ConnectorViewList *connector)
{
    this->connector_ = connector;
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

QString PinView::tip()
{
    QString pt("Unkown type");
    switch (model()->type()) {
    case PinModel::INPUT:
        pt = "Input pin";
        break;
    case PinModel::OUTPUT:
        pt = "Output pin";
        break;
    case PinModel::EPISODIC:
        pt = "Episodic pin";
        break;
    }

    return QString("<b>Pin %1</b><br><u>%2</u> (%3)<hr>" \
            "<b>Address:</b> 0x%4<br>" \
            "<b>Width:</b>%5 bits")
        .arg(model()->id())
        .arg(model()->name())
        .arg(pt)
        .arg(model()->address())
        .arg(model()->bits());
}

void PinView::deleteConnection()
{
    if (connector_ != 0) {
        delete connector_;
    }
}

void PinView::deleteView()
{
    deleteConnection();
    delete this;
}


BlockView *PinView::parent()
{
    return block_;
}
