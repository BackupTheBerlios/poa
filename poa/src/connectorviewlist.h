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
 * $Id: connectorviewlist.h,v 1.12 2003/11/24 20:11:59 squig Exp $
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
class ConnectorViewList : public QObject//, public Serializable
{
    Q_OBJECT

public:

    /**
     * Creates a connector view list on the given <code>canvas</code> and
     * draws a routed line from <code>source</code> pin to <code>target</code>
     * pin for the given <code>model</code>.
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
     * Serializes this instance to a xml subtree.
     * @param document the main QDomDocument instance. Needed to create
     *        elements
     */
    virtual QDomElement serialize(QDomDocument *);

protected:

    /**
     * Changes <code>this</code> to contain the segments connecting
     * <code>points</code>
     */
    void applyPointList(const QValueList<QPoint> &list);

    /**
     * Deserializes an xml subtree to recreate the point list.
     * <code>source</code> and <code>target</code> are ignored
     */
    virtual void deserialize(QDomElement *element);


    /**
     * Routes a point list.
     */
    static QValueList<QPoint> *routeConnector(QPoint from,
                                              LineDirection fromDir,
                                              QPoint to,
                                              LineDirection toDir);

    /**
     * Routes a point list. Tries to minimize (1) collisions, (2) inflection
     * points.
     */
    QValueList<QPoint> *routeOptimizing(QPoint from,
                                        LineDirection fromDir,
                                        QPoint to,
                                        LineDirection toDir);

    unsigned weight(const QValueList<QPoint> &points);

    unsigned lineWeight(QPoint from, QPoint to);

private:

    /** Source pin view */
    PinView *source_;
    /** Target pin view */
    PinView *target_;
    /** the current canvas */
    QCanvas *canvas_;
    /** List of all segments of this connector view */
    // FIX: this list should contain ConnectorViewSegment objects only
    QCanvasItemList segments_;

public slots:

    /**
     * Deletes (and frees) all segments.
     */
    void deleteAllConnectorViews();

    /**
     * To be called when one of the pin is destroyed
     */
    void deleteView();

private slots:
    /**
     * Will be connected to the incident pin views' signal 'moved'.
     */
    void pinMoved(PinView*);

signals:
    void select(bool);
    void hover(bool);
};


#endif // POA_CONNECTORVIEWLIST_H
