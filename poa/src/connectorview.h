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
 * $Id: connectorview.h,v 1.4 2003/08/28 18:09:24 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA_CONNECTORVIEW_H
#define POA_CONNECTORVIEW_H

#include <qcanvas.h>

class ConnectorModel;
class PinView;


class ConnectorView;

/**
 * A <code>ConnectorView</code> can dock onto another connector view
 * or onto a pin.
 */
union ConnectorDocking {
    ConnectorView *connector;
    PinView *pin;
};

/*****************************************************************************
 * Definition of a connector view. Connector views are doubly linked linear
 * lists of <code>ConnectorView</code>s. The first item in that list is
 * considered the view-object and listens to changes in the model.
 *
 * Each item in the list represents either a horizontal or a vertical line.
 * The entire list connects the source pin to the target pin.
 *
 * Each item in the list propagates changes to itself along the list. During
 * propagation some items may be removed or new items may be inserted into
 * the list.
 */
class ConnectorView: public QCanvasLine
{

public:

    ConnectorView(PinView *from, ConnectorView *to, QCanvas *canvas);
    ConnectorView(ConnectorView *from, PinView *to, QCanvas *canvas);
    ConnectorView(PinView *from, PinView *to, QCanvas *canvas);
    ConnectorView(ConnectorView *from, ConnectorView *to, QCanvas *canvas);

    virtual ~ConnectorView();

    /*************************************************************************
     * Returns <code>this</code>'s model
     */
    ConnectorModel *model();

private:

    /** model for <code>this</code> */
    ConnectorModel *model_;

    /** true if <code>this</code> has a pin as source, false else */
    bool first_;
    /** true if <code>this</code> has a pin as target, false else */
    bool last_;
    /** <code>this</code>'s source item */
    ConnectorDocking prev_;
    /** <code>this</code>'s target item */
    ConnectorDocking next_;

};


#endif // POA_CONNECTORMODELVIEW_H
