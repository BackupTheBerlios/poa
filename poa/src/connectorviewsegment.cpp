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
 * $Id: connectorviewsegment.cpp,v 1.7 2003/11/26 16:27:10 squig Exp $
 *
 *****************************************************************************/

#include "connectorviewlist.h"
#include "connectorviewsegment.h"
#include "gridcanvas.h"
#include "project.h"

#include <qcanvas.h>

ConnectorViewSegment::ConnectorViewSegment(QPoint from,
                                           QPoint to,
                                           QCanvas *canvas,
                                           ConnectorViewList *viewList)
    : QCanvasLine(canvas)
{
    viewList_ = viewList;
    setPoints(from.x(), from.y(), to.x(), to.y());
    updateProperties();
}

ConnectorViewSegment::~ConnectorViewSegment()
{
}

void ConnectorViewSegment::remove(Project *)
{
    // delete segment from list and mem
    viewList_->deleteSegment(this);
    // if list is empty, delete it
    if (viewList_->allSegments().empty()) {
        delete viewList_;
    }
}

QString ConnectorViewSegment::tip()
{
    return viewList()->tip();
}

ConnectorViewList *ConnectorViewSegment::viewList()
{
    return viewList_;
}

void ConnectorViewSegment::setSelected(bool yes)
{
    viewList_->setSelected(yes);
}

void ConnectorViewSegment::select(bool yes)
{
    QCanvasLine::setSelected(yes);
    updateProperties();
}

void ConnectorViewSegment::updateProperties()
{
    if (isActive()) {
        setPen(QPen(Settings::instance()->activatedColor(), 2));
    }
    else if (isSelected()) {
        setZ(static_cast<GridCanvas *>(canvas())->incZ());
        setPen(QPen(Settings::instance()->selectedColor(), 2));
    }
    else {
        setZ(0);
        setPen(QPen(Settings::instance()->defaultColor(), 2));
    }

}

