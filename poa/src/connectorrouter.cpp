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
 * $Id: connectorrouter.cpp,v 1.6 2004/01/30 14:24:59 keulsn Exp $
 *
 *****************************************************************************/


#include "blockview.h"
#include "connectorrouter.h"
#include "connectorviewsegment.h"
#include "connectorviewlist.h"
#include "pinview.h"

#include <qcanvas.h>
#include <qptrdict.h>

void ConnectorRouter::route(QCanvasItemList items)
{
    QPtrDict<ConnectorViewList> routeItems;

    QCanvasItemList::iterator it = items.begin();
    for (; it != items.end(); ++it) {
        ConnectorViewSegment *segment
            = dynamic_cast<ConnectorViewSegment *>(*it);
        if (segment != 0) {
            routeItems.replace(segment->viewList(), segment->viewList());
        }
        else {
            PinView *pin = dynamic_cast<PinView *>(*it);
            if (pin != 0) {
                if (pin->isConnected()) {
                    routeItems.replace(pin->connector(), pin->connector());
                }
            }
            else {
                BlockView *block = dynamic_cast<BlockView *>(*it);
                if (block != 0) {
                    add(&routeItems, block);
                }
            }
        }
    }

    QValueList<ConnectorViewList*> connectors;
    QPtrDictIterator<ConnectorViewList> it2(routeItems);
    for(; it2.current(); ++it2 ) {
        //connectors.append(it2.current()); why? --> linear search (sk 30/01)
	connectors.prepend(it2.current());
	(it2.current())->awaitRerouting(true);
    }
    route(connectors);
}

void ConnectorRouter::add(QPtrDict<ConnectorViewList> *routeItems,
                          BlockView *block)
{
    QValueList<PinView *> pins = block->pins();
    QValueListIterator<PinView *> it = pins.begin();
    for (; it != pins.end(); ++it) {
        if ((*it)->isConnected()) {
            routeItems->replace((*it)->connector(), (*it)->connector());
        }
    }
}
