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
 * $Id: path.cpp,v 1.1 2004/02/18 03:42:19 keulsn Exp $
 *
 *****************************************************************************/


#include "path.h"

#include <qptrlist.h>
#include <qvaluelist.h>
#include <limits.h>


class DepthFirstNode
{
public:
    DepthFirstNode(BlockNode *block);

    void addPredecessor(DepthFirstNode *node);
    const DepthFirstNodeList getPredecessors() const;

    BlockNode *node() const;

    int time;
    int lowest;

private:
    BlockNode *block_;
    DepthFirstNodeList pred_;
};

typedef QMap<BlockNode*, DepthFirstNode*> DepthFirstMap;
typedef QMapIterator<BlockNode*, DepthFirstNode*> DepthFirstMapIterator;



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
	    offset = (*it)->runtime() + 1;
	    (*it)->setFlag(true);
	}
	else {
	    offset = (*it)->offset() + (*it)->runtime() + 1;
	}
	// set offset going forward
	QValueList<BlockNode*>::iterator tmp = it;
	while (++it != nodes_.end()) {
	    (*it)->setOffset(offset % (*it)->clock());
	    offset += (*it)->runtime() + 1;
	    (*it)->setFlag(true);
	}
	// set offset going backward
	it = tmp;
	offset = (*it)->offset();
	while (it != nodes_.begin()) {
	    unsigned reduction = (*it)->runtime() + 1;
	    --it;
	    if ((*it)->flag()) {
		// block already has an offset
		offset = (*it)->offset();
	    }
	    else {
		// block needs new offset
		if (reduction > offset) {
		    offset = (reduction - offset) % (*it)->clock();
		}
		else {
		    offset = (offset - reduction) % (*it)->clock();
		}
		(*it)->setOffset(offset);
		(*it)->setFlag(true);
	    }
	}
    }
}

bool Path::higherPriority(const PriorityItem *other) const {
    const Path *otherPath = dynamic_cast<const Path*>(other);
    Q_ASSERT(otherPath != 0);
    return this->runtime_ > otherPath->runtime_;
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
    int time;
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
