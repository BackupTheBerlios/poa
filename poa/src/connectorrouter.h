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
 * $Id: connectorrouter.h,v 1.5 2004/02/19 16:09:10 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA_CONNECTOR_ROUTER_H
#define POA_CONNECTOR_ROUTER_H

#include "router.h"

#include <qcanvas.h>
#include <qptrdict.h>
#include <qvaluelist.h>

class BlockView;
class ConnectorViewList;

/**
 * Objects of this class can route connector views.
 */
class ConnectorRouter : public Router
{
public:
    /**
     * Inherited from <code>Router</code>.
     */
    virtual void route(QCanvasItemList items);

    /**
     * Routes one <code>ConnectorViewList</code>
     */
    virtual void route(ConnectorViewList *view) = 0;

    /**
     * Routes a set of <code>ConnectorViewList</code>s performing egoistic
     * routing.
     */
    virtual void route(QValueList<ConnectorViewList*>& items);

private:
    /**
     * Adds the connectors of all pins of block to routeItems.
     */
    void add(QPtrDict<ConnectorViewList> *routeItems, BlockView *block);

};

#endif // POA_CONNECTOR_ROUTER_H

