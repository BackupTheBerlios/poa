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
 * $Id: path.h,v 1.3 2004/03/05 14:48:24 keulsn Exp $
 *
 *****************************************************************************/

#ifndef POA_PATH_H
#define POA_PATH_H

#include <qmap.h>

#include "blockgraph.h"
#include "blockmodel.h"
#include "genericpriorityqueue.h"


class DepthFirstNode;
typedef QMap<BlockNode*, DepthFirstNode*> BlockMap;
typedef QValueList<DepthFirstNode*> DepthFirstNodeList;

class Path;
typedef GenericPriorityQueue<Path> PathQueue;


/**
 * Instances of this class define paths in the block graph. The time
 * it takes one signal to traverse such a path can be minimized using
 * an instance of this class.
 *
 * Instances of this class can be inserted into a <code>PathQueue</code> they
 * are sorted longest runtime first.
 */
class Path : public PriorityItem
{
public:
    /**
     * Creates a path containing one destination node. More nodes can
     * be added using {@link #prepend}.
     */
    Path(BlockNode *target);

    /**
     * Copy constructor.
     */
    Path(const Path &other);

    /**
     * Default destructor.
     */
    virtual ~Path();

    /**
     * Prepends the path currently stored in this by one node. That node
     * must have a connector to the first node in this path.
     * @param node New first node in this path. Must be != 0
     */
    void prepend(BlockNode *node);

    /**
     * Removes the first node from this path, if this is not the empty path,
     * otherwise does nothing.
     */
    void removeFirst();

    /**
     * Sets the {@link BlockNode#setFlag} property on all nodes in this path
     * to <code>state</code>.
     */
    void setNodeFlag(bool state);

    /**
     * Returns a user-presentable description of this path.
     */
    QString getText() const;

    /**
     * Returns the number of nodes on this path.
     */
    unsigned length() const;

    /**
     * Returns the node that has distance <code>add</code> steps
     * to the first node in this path. If no such node exists, then the
     * behavior and result of this method are undefined.
     */
    const BlockNode *front(int add) const;

    /**
     * Returns the node that has follows <code>add</code> steps after
     * the last node in this path. If no such node exists, then the
     * behavior and result of this method are undefined. Note values
     * <code>add > 0</code> do not make sense.
     */
    const BlockNode *end(int add) const;

    /**
     * Tries to minimize the time a signal needs to traverse this path.
     * Sets the offset on all nodes that have their {@link BlockNode#flag}
     * property set to <code>false<code>. Does not touch nodes that have the
     * flag set. Afterwards sets the flag on all nodes in this path. <br>
     * Postcondition: all nodes on this path have their flag set.
     */
    void optimize();

    /**
     * Defines a total ordering on all paths. A path with a higher traversal
     * time has a higher priority.
     */
    virtual bool higherPriority(const PriorityItem *other) const;

    /**
     * Calculates all acyclic paths starting at node <code>from</code> and
     * ending in node <code>to</code>. If any such path contains a cycle,
     * then the last edge on that cycle is ignored. Thus all resulting
     * paths (if any) form a DAG; either an empty DAG or one with one single
     * source <code>from</code> and one single target <code>to</code>. Inserts
     * one <code>Path</code>-object per path found into <code>paths</code>.
     * Note that when successively removing all paths from that queue, the
     * paths are provided in monotonously descending traversal time.
     * @see #higherPriority
     */
    static void allPaths(PathQueue &paths,
                         BlockNode *from,
                         BlockNode *to);

private:

    /**
     * Constant value for a very large integer value, used in Tarjan's
     * algorithm.
     */
    static const int infinity;

    /**
     * Implements a modified version of Tarjan's algorithm for finding
     * strongly connected components in a directed graph. This implementation
     * builds a DAG of {@link DepthFirstNode}-objects. The DAG has a target
     * node as its "root" and contains a subset of the edges contained in the
     * reversed blockgraph. <br>
     * Expects some variables to be passed by reference. The value of those
     * variables is meaningless after the method has returned, except for
     * <code>blockMap</code>
     * @param current    Reference to the Source node, where the depth
     *                   first search starts.
     * @param time       Reference to an integer variable, should be
     *                   initialized to 0.
     * @param blockMap   Reference to a mapping of the pointers to the block
     *                   graph nodes to the newly created DepthFirstNodes. Must
     *                   contain the respective pair for <code>current</code>.
     *                   After the method has returned, the target node of
     *                   can be found in the blockMap, otherwise there exists
     *                   no path from current to that target node.
     * @param cycleStack Reference to an empty list.
     *
     */
    static void recursiveTarjan(DepthFirstNode &current,
                                int &time,
                                BlockMap &blockMap,
                                DepthFirstNodeList &cycleStack);

    /**
     * Extracts all paths from the DAG with "root" <code>latest</code>
     * and inserts them into <code>queue</code>. Needs a variable
     * <code>current</code> that has target node <code>latest->toNode()</code>
     * and no other nodes.
     */
    static void extractPaths(PathQueue &paths,
                             const DepthFirstNode &latest,
                             Path &current);

    /** List of nodes starting from source, ending in target node */
    QValueList<BlockNode*> nodes_;
    /** Sum of runtimes of all nodes in this path */
    unsigned int runtime_;
};


#endif // POA_PATH_H
