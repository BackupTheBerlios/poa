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
 * $Id: connectorview.h,v 1.12 2003/09/18 01:51:17 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA_CONNECTORVIEW_H
#define POA_CONNECTORVIEW_H

#include <qcanvas.h>

#include "tooltipable.h"
#include "pinview.h"
#include "poa.h"


class ConnectorView;


/**
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
class ConnectorView: public QCanvasLine, public Tooltipable
{

public:
    /**
     * Creates a connector view on the given <code>canvas</code> and draws a
     * routed line from <code>from</code> pin to <code>to</code> pin.
     */
    ConnectorView(PinView *from,
		  PinView *to,
		  QCanvas *canvas);

    ConnectorView(PinView *from,
		  QPoint to,
		  LineDirection toDir,
		  QCanvas *canvas);

    /**
     * Default destructor
     */
    virtual ~ConnectorView();

    /**
     * Returns a list including <code>this</code> and all succeeding view
     * objects. That is the list of all objects that make up a connector
     * view on a canvas.
     */
    QCanvasItemList allSegments();

    /**
     * Returns the tooltip text
     */
    QString tip();


    PinView *from();

    PinView *to();

protected:
    ConnectorView(QPoint first,
		  QPoint second,
		  QCanvas *canvas);

    void applyPointList(QValueList<QPoint> *list,
			PinView *targetPin = 0);

    void destroySuccessors();

    bool canUseDir(LineDirection goDir,
		   bool honorGoDir,
		   LineDirection dir);

    /*    QValueList<QPoint> *routeUsingLastButOne(QPoint startPoint,
					     LineDirection startDir,
					     bool honorStartDir,
					     QPoint lastButOne,
					     QPoint endPoint);

    QValueList<QPoint> *routeConnector(QPoint startPoint,
				       LineDirection startDir,
				       bool honorStartDir,
				       QPoint endPoint,
				       LineDirection endDir,
				       bool honorEndDir);*/
    QValueList<QPoint> *routeConnector(QPoint from,
				       LineDirection fromDir,
				       QPoint to,
				       LineDirection toDir);


    void setPrevConnector(ConnectorView *prev);

    void setNextConnector(ConnectorView *next);

    void setPrevPin(PinView *source);

    void setNextPin(PinView *target);

    /**
     * A <code>ConnectorView</code> can dock onto another connector view
     * or onto a pin.
     */
    union ConnectorDocking {
        ConnectorView *connector;
        PinView *pin;
    };

    static const unsigned DEFAULT_DOCK_LINE_LENGTH = 5;

private:
    /**
     * model for <code>this</code> if <code>this</code> is the first view
     * in the list, 0 else.
     */
    PinView *from_;
    PinView *to_;

    /**
     * true if <code>this</code> has a pin as source or no source,
     * false else
     */
    bool first_;

    /**
     * true if <code>this</code> has a pin as target or no target,
     * false else
     */
    bool last_;

    /**
     * <code>this</code>'s source item or 0 if <code>this</code> has no
     * source
     */
    ConnectorDocking prev_;

    /**
     * <code>this</code>'s target item or 0 if <code>this</code> has no target
     */
    ConnectorDocking next_;

};


#endif // POA_CONNECTORMODELVIEW_H
