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
 * $Id: connectorrouter.cpp,v 1.2 2003/12/04 10:28:15 squig Exp $
 *
 *****************************************************************************/


#include "connectorrouter.h"
#include "connectorviewsegment.h"

#include <qcanvas.h>

#include <pair.h>
#include <map>


void ConnectorRouter::route(QCanvasItemList items)
{
    typedef std::map<ConnectorViewSegment*, ConnectorViewSegment*> ConnectorHistory;

    ConnectorHistory history;
    QCanvasItemList::iterator it = items.begin();
    for (; it != items.end(); ++it) {
        ConnectorViewSegment *view = dynamic_cast<ConnectorViewSegment*>(*it);
        if (view != 0) {
            std::pair<ConnectorHistory::iterator, bool> result =
              history.insert(ConnectorHistory::value_type(view, view));

            if (result.second) {
                route(view->viewList());
            }
        }
    }
}
