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
 * $Id: connectorview.cpp,v 1.1 2003/08/28 18:09:24 keulsn Exp $
 *
 *****************************************************************************/


#include "connectorview.h"


ConnectorView::ConnectorView(PinView *from, ConnectorView *to, QCanvas *canvas)
    : QCanvasLine(canvas)
{
    first_ = true;
    last_ = false;
    prev_.pin = from;
    next_.connector = to;
}

ConnectorView::ConnectorView(ConnectorView *from, PinView *to, QCanvas *canvas)
    : QCanvasLine(canvas)
{
    first_ = false;
    last_ = true;
    prev_.connector = from;
    next_.pin = to;
}

ConnectorView::ConnectorView(PinView *from, PinView *to, QCanvas *canvas)
    : QCanvasLine(canvas)
{
    first_ = true;
    last_ = true;
    prev_.pin = from;
    next_.pin = to;
}

ConnectorView::ConnectorView(ConnectorView *from,
			     ConnectorView *to,
			     QCanvas *canvas)
    : QCanvasLine(canvas)
{
    first_ = false;
    last_ = false;
    prev_.connector = from;
    next_.connector = to;
}

ConnectorView::~ConnectorView()
{
}

ConnectorModel *ConnectorView::model()
{
    return model_;
}

