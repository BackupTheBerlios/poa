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
 * $Id: connectorviewlist.h,v 1.1 2003/09/16 12:17:51 garbeam Exp $
 *
 *****************************************************************************/


#ifndef POA_CONNECTORVIEWLIST_H
#define POA_CONNECTORVIEWLIST_H

#include "connectorview.h"

#include <qcanvas.h>
#include <qobject.h>

/**
 * Definition of a doubly linked connector view list which is used to manage
 * all parts (ConnectorViews) of a connection between two pins.
 *
 * Each item in the list represents either a horizontal or a vertical line.
 *
 * Each item in the list propagates changes to itself along the list. During
 * propagation some items may be removed or new items may be inserted into
 * the list.
 */
class ConnectorViewList : public QObject, public QCanvasItemList
{
    Q_OBJECT

public:

    /**
     * Creates a connector view list on the given <code>canvas</code> and draws a
     * routed line from <code>source</code> pin to <code>target</code> pin for the
     * given <code>model</code>.
     */
    ConnectorViewList(PinView *source, PinView *target);

    /**
     * Default destructor
     */
    virtual ~ConnectorViewList();

    /**
     * Returns a list including <code>this</code> and all succeeding view
     * objects. That is the list of all objects that make up a connector
     * view on a canvas.
     */
    ConnectorViewList *allSegments();

    /**
     * Returns the source pin.
     */
    PinView *source();

    /**
     * Returns the target pin.
     */
    PinView *target();

private:

    /** source pin view */
    PinView *source_;
    /** target pin view */
    PinView *target_;

public slots:

    /**
     * Deletes (and frees) this ConnectorViewList and its
     * ConnectorView items.
     */
    void deleteAllConnectorViews();
};


#endif // POA_CONNECTORVIEWLIST_H
