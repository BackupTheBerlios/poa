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
 * $Id: directrouter.h,v 1.3 2003/12/11 15:40:10 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA_DIRECT_ROUTER
#define POA_DIRECT_ROUTER

#include "connectorrouter.h"
#include "poa.h"


class ConnectorViewList;


class DirectRouter : public ConnectorRouter
{

    /**
     * Routes a <code>ConnectorViewList</code>.
     */
    virtual void route(ConnectorViewList *view);

    /**
     * Routes a set of <code>ConnectorViewList</code>s
     */
    virtual void route(QValueList<ConnectorViewList*>& list);

protected:

    /**
     * Calculates direct routing for connector from a point and starting
     * direction to a different point and target direction. Returns a list
     * of 2 or more {@link QPoint}-objects to be connected by lines.
     */
    QValueList<QPoint> *routeOne(QPoint from,
				 LineDirection fromDir,
				 QPoint to,
				 LineDirection toDir);

};


#endif // POA_DIRECT_ROUTER
