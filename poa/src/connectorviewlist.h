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
 * $Id: connectorviewlist.h,v 1.17 2004/01/13 15:04:37 vanto Exp $
 *
 *****************************************************************************/


#ifndef POA_CONNECTORVIEWLIST_H
#define POA_CONNECTORVIEWLIST_H

#include <qcanvas.h>
#include <qobject.h>
class QDomDocument;
class QDomElement;

#include "poa.h"
#include "serializable.h"
#include "connectorviewsegment.h"
//class ConnectorViewSegment;
class GridCanvas;
class PinView;

/**
 * Definition of a doubly linked connector view list which is used to manage
 * all parts (ConnectorViewSegments) of a connection between two pins.
 *
 * Each item in the list represents either a horizontal or a vertical line.
 *
 * Each item in the list propagates changes to itself to an instance of
 * <code>this</code>. During propagation some items may be removed or new
 * items may be inserted into the list.
 */
class ConnectorViewList : public QObject//, public Serializable
{
    Q_OBJECT

public:

    /**
     * Creates a connector view list on the given <code>canvas</code> with
     * empty segment list.
     */
    ConnectorViewList(PinView *source,
                      PinView *target,
                      GridCanvas *canvas,
                      QDomElement *element = 0);

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
     * Removes and frees a segment
     */
    void deleteSegment(ConnectorViewSegment *);

    /**
     * Returns the points connected by all segments in <code>this</code> list.
     */
    QValueList<QPoint> points();

    /**
     * Returns the tool tip text for this connector view.
     */
    QString tip();

    /**
     * Selects/Deselects this connector (all connector segments).
     */
    void setSelected(bool);

    /**
     * Does nothing, the view list is never displayed.
     */
    virtual void setVisible(bool visible);

    /**
     * Serializes this instance to an xml subtree.
     * @param document the main QDomDocument instance. Needed to create
     *        elements
     */
    virtual QDomElement serialize(QDomDocument *);

    /**
     * Changes <code>this</code> to contain the segments connecting
     * <code>points</code>
     */
    void applyPointList(const QValueList<QPoint> &list);


    /**
     * Returns the canvas <code>this</code>'s segments are drawn onto.
     */
    QCanvas *canvas() const;

    //protected:

    /**
     * Deserializes an xml subtree to recreate the point list.
     * <code>source</code> and <code>target</code> are ignored
     */
    virtual void deserialize(QDomElement *element);

private:

    /** Source pin view */
    PinView *source_;
    /** Target pin view */
    PinView *target_;
    /** the current canvas */
    QCanvas *canvas_;
    /** List of all segments of this connector view */
    QPtrList<ConnectorViewSegment> segments_;

signals:
    void select(bool);
    void hover(bool);
};


#endif // POA_CONNECTORVIEWLIST_H
