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
 * $Id: connectorviewlist.h,v 1.5 2003/09/20 21:14:48 squig Exp $
 *
 *****************************************************************************/


#ifndef POA_CONNECTORVIEWLIST_H
#define POA_CONNECTORVIEWLIST_H

#include <qcanvas.h>
#include <qobject.h>

#include "poa.h"

class ConnectorViewSegment;
class GridCanvas;
class PinView;


/**
 * Definition of a doubly linked connector view list which is used to manage
 * all parts (ConnectorViews) of a connection between two pins.
 *
 * Each item in the list represents either a horizontal or a vertical line.
 *
 * Each item in the list propagates changes to itself to an instance of
 * <code>this</code>. During propagation some items may be removed or new
 * items may be inserted into the list.
 */
class ConnectorViewList : public QObject
{
    Q_OBJECT

public:

    /**
     * Creates a connector view list on the given <code>canvas</code> and
     * draws a routed line from <code>source</code> pin to <code>target</code>
     * pin for the given <code>model</code>.
     */
    ConnectorViewList(PinView *source, PinView *target, GridCanvas *canvas);

    /**
     * Creates a connector view list on the given <code>canvas</code> and
     * inserts connector view segments connecting the given <code>points</code>
     */
    ConnectorViewList(PinView *source,
                      PinView *target,
                      const QValueList<QPoint> &points,
                      GridCanvas *canvas);

    /**
     * Default destructor
     */
    virtual ~ConnectorViewList();

    /**
     * Returns the source pin.
     */
    PinView *source();

    /**
     * Returns the target pin.
     */
    PinView *target();

    /**
     * Returns all segments in this list.
     */
    const QCanvasItemList allSegments();

    /**
     * Returns the points connected by all segments in <code>this</code> list.
     */
    QValueList<QPoint> points();

    /**
     * Returns the tool tip text for this connector view.
     */
    QString tip();

    /**
     */
    virtual void serialize();


protected:

    /**
     * Changes <code>this</code> to contain the segments connecting
     * <code>points</code>
     */
    void applyPointList(const QValueList<QPoint> &list, QCanvas *canvas);

    /**
     * Routes a point list.
     */
    static QValueList<QPoint> *routeConnector(QPoint from,
                                              LineDirection fromDir,
                                              QPoint to,
                                              LineDirection toDir);


private:

    /** Source pin view */
    PinView *source_;
    /** Target pin view */
    PinView *target_;

    /** List of all segments of this connector view */
    QCanvasItemList segments_;

public slots:

    /**
     * Deletes (and frees) all segments.
     */
    void deleteAllConnectorViews();

    /**
     * To be called when one of the pin views is destroyed
     */
    void deleteView(PinView *);

};


#endif // POA_CONNECTORVIEWLIST_H
