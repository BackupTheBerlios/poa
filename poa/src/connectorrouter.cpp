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
 * $Id: connectorrouter.cpp,v 1.5 2004/01/09 16:56:24 squig Exp $
 *
 *****************************************************************************/


#include "blockview.h"
#include "connectorrouter.h"
#include "connectorviewsegment.h"
#include "connectorviewlist.h"
#include "pinview.h"

#include <qcanvas.h>
#include <qptrdict.h>
// #include <utility>
// #include <map>
// #include <qdict.h>

void ConnectorRouter::route(QCanvasItemList items)
{
//     typedef std::map<ConnectorViewSegment*, ConnectorViewSegment*> ConnectorHistory;

//     ConnectorHistory history;
//         ConnectorViewSegment *view = dynamic_cast<ConnectorViewSegment*>(*it);
//         if (view != 0) {
//             std::pair<ConnectorHistory::iterator, bool> result =
//               history.insert(ConnectorHistory::value_type(view, view));

//             if (result.second) {
//                 route(view->viewList());
//             }
//         }
//     }
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
        connectors.append(it2.current());
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
