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
 * $Id: dijkstrarouter.h,v 1.5 2004/01/09 16:56:24 squig Exp $
 *
 *****************************************************************************/


#ifndef POA_DIJKSTRA_ROUTER
#define POA_DIJKSTRA_ROUTER

#include <qcanvas.h>
#include <qpoint.h>

#include "connectorrouter.h"
#include "poa.h"
#include "priorityqueue.h"


class DijkstraRouter : public ConnectorRouter
{
public:

    /**
     * Routes one {@link ConnectorViewList}s with an egoistic routing
     * based on the single source all targets shortest path algorigm
     * by Dijkstra
     */
    virtual void route(ConnectorViewList *view);

    /**
     * Routes all {@link ConnectorViewList}s in <code>items</code> in their
     * order. Each one of those objects tries to minimize its number of
     * inflection points and collisions with other items
     */
    virtual void route(QValueList<ConnectorViewList*>& items);

protected:
    virtual QValueList<QPoint> *routeOne(QPoint from,
                     LineDirection fromDir,
                     QPoint to,
                     LineDirection toDir);
};



/**
 * Instances of this class represent nodes in the graph that  Dijkstra's one
 * source all targets shortest path algorithm is applied to.
 */
class Node
{
public:
    /**
     * Constructs a node at a point with an outgoing direction.
     * @param point The node's point
     * @param dir The direction any edge <code>e</code> must follow, if
     *        <code>e</code>'s source is <code>this</code>.
     */
    Node(QPoint point, LineDirection dir);

    /**
     * Strict total order on <code>Node</code>
     * (antisymmetric, irreflexive, transitive).
     */
    bool operator<(const Node &other) const;
    /**
     * Equivalence on <code>Node</code> induced by "less than" operator
     * (reflexive, transitive, symmetric).
     */
    bool operator==(const Node &other) const;
    bool operator!=(const Node &other) const;

    /**
     * Returns the Point this node lies on.
     */
    QPoint point() const;

    /**
     * Returns the direction any outgoing edge must have.
     */
    LineDirection dir() const;

private:
    /** Point reached */
    QPoint point_;
    /** Direction in that <code>point</code> was reached */
    LineDirection dir_;
};



class PossiblePoint : public PriorityItem
{
public:

    PossiblePoint(Node node);
    PossiblePoint(PossiblePoint *prev, Node node);

    virtual bool higherPriority(const PriorityItem *other) const;

    Node node() const;

    LineDirection direction() const;

    QPoint point() const;

    unsigned weight() const;

    void reach(PossiblePoint *prev);

    PossiblePoint *prev() const;

    unsigned lineWeight(QPoint from, QPoint to) const;

    static QCanvas *canvas_;

private:

    void updateWeight();

    unsigned calcWeight(PossiblePoint *prev) const;


    Node node_;
    PossiblePoint *prev_;
    unsigned weight_;
};


#endif // POA_DIJKSTRA_ROUTER
