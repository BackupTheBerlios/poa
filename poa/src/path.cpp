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
 * $Id: path.cpp,v 1.4 2004/03/07 17:39:40 keulsn Exp $
 *
 *****************************************************************************/


#include "path.h"

#include <qptrlist.h>
#include <qvaluelist.h>
#include <limits.h>


/**
 * Instances of this class make up a reverse-DAG (depth-first tree with
 * additional cross-edges, cycles are not allowed by convention).
 * Such objects are used by {@link #recursiveTarjan}.
 */
class DepthFirstNode
{
public:
    /**
     * Creates a DAG-Node that correspondes to the block node
     * <code>block</code>
     */
    DepthFirstNode(BlockNode *block);

    /**
     * Adds an edge from this to <code>node</code>. In the block graph there
     * is an edge <code>e</code> from <code>node</code> to this, and
     * <code>e</code> does not create a cycle in the DAG.
     */
    void addPredecessor(DepthFirstNode *node);

    /**
     * Returns a list of all nodes that were added by {@link #addPredecessor}.
     */
    const DepthFirstNodeList getPredecessors() const;

    /**
     * Returns a pointer to the block node this corresponds to.
     */
    BlockNode *node() const;

    /**
     * Point of time when this was visited by Tarjan's algorithm. Set to
     * -1 when this is constructed, may later be modified by Tarjan.
     */
    int time;

    /**
     * Smallest point of time among all nodes that can be reached starting at
     * this. Tarjan's algorithm uses this to find strongly connected
     * components. Initially set to INT_MAX, may later be modified by Tarjan.
     */
    int lowest;

private:
    /** Pointer to the block node this corresponds to. */
    BlockNode *block_;
    /** Predecessors of this */
    DepthFirstNodeList pred_;
};



/******************
 * DepthFirstNode *
 ******************/

DepthFirstNode::DepthFirstNode(BlockNode *block)
{
    block_ = block;
    time = -1;
    lowest = INT_MAX;
}

void DepthFirstNode::addPredecessor(DepthFirstNode *node)
{
    pred_.prepend(node);
}

const DepthFirstNodeList DepthFirstNode::getPredecessors() const
{
    return pred_;
}

BlockNode *DepthFirstNode::node() const
{
    return block_;
}


/********
 * Path *
 ********/

Path::Path(BlockNode *target) {
    runtime_ = 0;
    prepend(target);
}

Path::Path(const Path &other)
    : PriorityItem()
{
    runtime_ = other.runtime_;
    nodes_ = other.nodes_;
}

Path::~Path()
{
}

void Path::prepend(BlockNode *node) {
    Q_ASSERT(node != 0);
    nodes_.prepend(node);
    runtime_ += node->model()->runtime();
    updatePriority();
}

void Path::removeFirst()
{
    if (!nodes_.isEmpty()) {
        BlockNode *first = nodes_.front();
        nodes_.pop_front();
        runtime_ -= first->runtime();
        updatePriority();
    }
}

void Path::setNodeFlag(bool state)
{
    QValueList<BlockNode*>::iterator it = nodes_.begin();
    for (; it != nodes_.end(); ++it) {
        (*it)->setFlag(state);
    }
}

unsigned Path::length() const
{
    return nodes_.size();
}

const BlockNode *Path::front(int add) const
{
    return nodes_[0 + add];
}

const BlockNode *Path::end(int add) const
{
    return nodes_[nodes_.size() - 1 + add];
}

void Path::optimize()
{
    if (nodes_.size() > 0) {
        unsigned offset;
        // find last block that has an offset, if such a block exists, else
        // find first block
        QValueList<BlockNode*>::iterator it = nodes_.end();
        while (--it != nodes_.begin() && !(*it)->flag());
        if (!(*it)->flag()) {
            Q_ASSERT(it == nodes_.begin());
            // when starting from first block, then assign offset 0 to the
            // first block
            (*it)->setOffset(0);
            (*it)->setFlag(true);
        }
        offset = (*it)->offset() + (*it)->runtime() + 1;
        // set offset going forward
        QValueList<BlockNode*>::iterator tmp = it;
        while (++it != nodes_.end()) {
            unsigned clock = (*it)->clock();
            if (clock != 0) {
                (*it)->setOffset(offset % clock);
            }
            else {
                // this should never happen, since every block needs a clock,
                // but in any case it is easy to deal with this special case
                (*it)->setOffset(offset);
            }
            offset += (*it)->runtime() + 1;
            (*it)->setFlag(true);
        }
        // set offset going backward
        it = tmp;
        offset = (*it)->offset();
        while (it != nodes_.begin()) {
            unsigned nextClock = (*it)->clock();
            --it;
            if ((*it)->flag()) {
                // block already has an offset
                offset = (*it)->offset();
            }
            else {
                // block needs new offset
                unsigned reduction = (*it)->runtime() + 1;
                unsigned clock = (*it)->clock();
                if (reduction > offset) {
                    if (nextClock > 0) {
                        // shift to a period where sensible value is possible
                        unsigned n = (reduction - offset) / nextClock + 1;
                        offset = n * nextClock + offset - reduction;
                    }
                    else {
                        // here it is quite hard to determine sensible values
                        // but since a clock of 0 will never make sense, we
                        // can just as well set the offset to 0
                        offset = 0;
                    }
                }
                else {
                    offset -= reduction;
                }

                if (clock > 0) {
                    (*it)->setOffset(offset % clock);
                }
                else {
                    (*it)->setOffset(offset);
                }
                (*it)->setFlag(true);
            }
        }
    }
}

bool Path::higherPriority(const PriorityItem *other) const {
    const Path *otherPath = dynamic_cast<const Path*>(other);
    Q_ASSERT(otherPath != 0);
    if (this->runtime_ > otherPath->runtime_) {
	return true;
    }
    else {
	return this->runtime_ == otherPath->runtime_
	    && this->getText() > otherPath->getText();
    }
}

QString Path::getText() const
{
    QValueList<BlockNode*>::const_iterator it = nodes_.begin();
    QString text;
    if (it != nodes_.end()) {
        text = (*it)->model()->name();
        while (++it != nodes_.end()) {
            text += " - " + (*it)->model()->name();
        }
    }
    return text;
}


const int Path::infinity = INT_MAX;

void Path::recursiveTarjan(DepthFirstNode &current,
                           int &time,
                           BlockMap &blockMap,
                           DepthFirstNodeList &cycleStack)
{
    current.time = time;
    current.lowest = time;
    int min = time;
    ++time;

    cycleStack.push_front(&current);

    const QPtrList<BlockNode> allNeighbours = current.node()->neighbours();
    QPtrListIterator<BlockNode> adj(allNeighbours);
    BlockNode *nn;
    while ((nn = adj()) != 0) {
        DepthFirstNode *neighbour;
        BlockMap::iterator it = blockMap.find(nn);
        if (it != blockMap.end()) {
            neighbour = *it;
        }
        else {
            blockMap[nn] = neighbour = new DepthFirstNode(nn);
        }

        if (neighbour->time < 0) {
            // new node hit
            neighbour->addPredecessor(&current);
            recursiveTarjan(*neighbour, time, blockMap, cycleStack);
        }
        else if (neighbour->time <= current.time) {
            // upward edge in the depth first search-tree --> cycle detected,
            // ignore this edge
        }
        else {
            // cross-edge in the depth first tree
            Q_ASSERT(neighbour->time == infinity);
            neighbour->addPredecessor(&current);
        }

        if (neighbour->lowest < min) {
            min = neighbour->lowest;
        }
    }

    if (min < current.lowest) {
        // current has backward edges, but current is not the first node in
        // the cycle
        current.lowest = min;
    }
    else {
        // cycle completed
        DepthFirstNode *item;
        do {
            item = cycleStack.front();
            cycleStack.pop_front();
            item->time = infinity;
        } while (item != &current);
    }
}


void Path::extractPaths(PathQueue &paths,
                        const DepthFirstNode &latest,
                        Path &current)
{
    // use backtracking to find all paths
    DepthFirstNodeList preds = latest.getPredecessors();
    DepthFirstNodeList::const_iterator adj = preds.begin();
    if (adj == preds.end()) {
        // first node reached
        paths.insert(new Path(current));
    }
    else {
        do {
            DepthFirstNode *next = *adj;
            current.prepend(next->node());
            extractPaths(paths, *next, current);
            current.removeFirst();
        } while (++adj != preds.end());
    }

}


void Path::allPaths(PathQueue &paths,
                    BlockNode *from,
                    BlockNode *to)
{
    // Use a modification of Tarjan's algorithm to find all paths that do not
    // contain any cycle
    DepthFirstNode *fromNode = new DepthFirstNode(from);
    BlockMap blockMap;
    blockMap[from] = fromNode;
    int time = 0;
    DepthFirstNodeList cycleStack;

    Path::recursiveTarjan(*fromNode, time, blockMap, cycleStack);

    paths.clear();

    BlockMap::iterator it = blockMap.find(to);
    if (it != blockMap.end()) {
        DepthFirstNode *toNode = *it;

        Path current(toNode->node());
        Path::extractPaths(paths, *toNode, current);
    }
    else {
        // to is not reachable from source node from --> there are no paths
    }

    QValueList<DepthFirstNode*> values = blockMap.values();
    blockMap.clear();
    for (QValueListIterator<DepthFirstNode*> valueIt = values.begin();
         valueIt != values.end(); valueIt = values.remove(valueIt)) {

        delete *valueIt;
    }
}
