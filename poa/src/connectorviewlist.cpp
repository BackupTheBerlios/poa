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
 * $Id: connectorviewlist.cpp,v 1.3 2003/09/18 14:24:36 vanto Exp $
 *
 *****************************************************************************/


#include "connectorviewlist.h"
#include "gridcanvas.h"

ConnectorViewList::ConnectorViewList(PinView *source, PinView *target, GridCanvas *canvas)
{
    source_ = source;
    target_ = target;

    // TODO: create new parts of ConnectorViews
}

ConnectorViewList::ConnectorViewList(PinView *source,
                     PinView *target,
                     const QValueList<QPoint> &points, GridCanvas *canvas)
{
    source_ = source;
    target_ = target;
}

ConnectorViewList::~ConnectorViewList()
{
    deleteAllConnectorViews();
    // emit signal to pinviews?
}

PinView *ConnectorViewList::source()
{
    return source_;
}

PinView *ConnectorViewList::target()
{
    return target_;
}

void ConnectorViewList::deleteAllConnectorViews()
{
    ConnectorViewList::iterator it = begin();

    while (it != end()) {
        ConnectorView *view = (ConnectorView *)(*it);
        delete view;
        it = erase(it);
    }
}

void ConnectorViewList::serialize()
{
}
