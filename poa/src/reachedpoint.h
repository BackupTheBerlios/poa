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
 * $Id: reachedpoint.h,v 1.1 2003/09/26 14:19:00 keulsn Exp $
 *
 *****************************************************************************/


#ifndef POA__REACHEDPOINT_H
#define POA__REACHEDPOINT_H

#include "poa.h"

#include <qpoint.h>


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


/**
 * Class for Support of Dijkstra's single source, all targets shortest
 * path algorithm. Nodes reached in the graph are instances of this class,
 * consisting of a point and the direction in which that point was reached.
 *
 * The weight estimation can be stored in an instance of this class through
 * {@link #setBadness}. Predecessors to an instance can be stored using
 * {@link #setPredecessor}
 */
class ReachedPoint
{
public:

    /**
     * Sets <code>this</code>'s badness to 0 and the predecessor to 0.
     *
     * @param node Node reached
     */
    ReachedPoint(Node node, unsigned badness, ReachedPoint *predecessor);

    /**
     * Sets the predecessor.
     * @see #predecessor
     */
    void setPredecessor(ReachedPoint *predecessor);

    /**
     * Returns the precessor set by {@link #setPredecessor}
     */
    ReachedPoint *predecessor() const;

    /**
     * Sets a badness for <code>this</code>. That badness can be used to
     * build a strict order of <code>ReachedPoint</code>-objects.
     * @see #betterThan
     */
    void setBadness(unsigned badness);

    /**
     * Returns the current badness. This badness can be set by
     * {@link #setBadness}
     */
    unsigned badness() const;

    /**
     * Returns <code>true</code> if <code>other</code> has the same point
     * and the same direction as <code>this</code>. Note that this method
     * is totally different from the equality relation implied by
     * {@link #betterThan}.
     */
    bool sameNode(const ReachedPoint *other) const;

    /**
     * Returns <code>this->badness() < other->badness()</code>
     */
    bool betterThan(const ReachedPoint *other) const;

private:
    /** Node reached. */
    Node node_;
    /** Badness */
    unsigned badness_;
    /** Predecessor by that <code>this</code> was reached */
    ReachedPoint *predecessor_;
};


/**
 * Compare class to be used in C++ Standard library (template) classes.
 * Compares two {@link ReachedPoint}-objects according to their badness.
 */
class CompareReachedPoints
{
public:
    /**
     * Returns <code>left.betterThan(&right)</code>
     */
    bool operator()(const ReachedPoint &left, const ReachedPoint &right)
	{ left.betterThan(&right); }

};


#endif
