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
 * $Id: scheduler.cpp,v 1.1 2004/02/09 01:29:49 keulsn Exp $
 *
 *****************************************************************************/

#include "scheduler.h"

#include <qmap.h>


typedef QMap<BlockNode*, int> BlockMap;
typedef QValueList<BlockNode*> BlockList;
typedef pair<BlockNode*, BlockNode*> Edge;

class DepthFirstNode
{
    DepthFirstNode(BlockNode *block, DepthFirstNode *pred);
    
    bool isStaticPred(const DepthFirstNode *block) const;
    DepthFirstNode *prev() const;

    BlockNode *block_;
    DepthFirstNode *pred_;
};

typedef QMap<BlockNode*, DepthFirstNode*> DepthFirstMap;
typedef QMapIterator<BlockNode*, DepthFirstNode*> DepthFirstMapIterator;


class EdgeList : public QValueList<Edge>
{
public:
    void pushEdges(const BlockNode& node);
};


class BlockTraversal
{
public:
    BlockTraversal(PinNode *from, PinNode *to);

    bool variableOffset() const;

    BlockNode *block() const;
    BlockModel *model() const;

    void commit();

private:
    PinNode *in_;
    PinNode *out_;
    unsigned long offset_;
};


class Path : public PriorityItem
{
public:
    virtual bool higherPriority(const PriorityItem *other) const;

private:
    BlockTraversal *first_;
};



/************
 * EdgeList *
 ************/

void EdgeList::pushEdges(const BlockNode& node)
{
    const QPtrList<BlockNode> neighbours = node.neighbours();
    QPtrListIterator<BlockNode> current(neighbours);
    for (; current != 0; ++current) {
	this->prepend(Edge(node, current));
    }
}



/******************
 * BlockTraversal *
 ******************/

BlockTraversal::BlockTraversal(PinNode *from, PinNode *to)
{
    in_ = from;
    out_ = to;
    Q_ASSERT((from == 0 && to != 0 && to->isInput() || from != 0 && to == 0)
	     || (from != 0 && to != 0 && from->parent() == to->parent()));
}



/*************
 * Scheduler *
 *************/

Scheduler::Scheduler(BlockGraph *graph)
{
    graph_ = graph;
}

Scheduler::~Scheduler()
{
}

Result Scheduler::optimizeBetween(PinNode *from, PinNode *to)
{
    if (isCyclic()) {
	return CYCLIC;
    }
    // repeat
    //   find (probably) shortest path
    //   assign offsets
    // until is shortest path
    PathQueue freshPaths(findAllPaths(from, to));
    if (freshPaths.isEmpty()) {
	return PARTITIONED;
    }
    PathQueue triedPaths;
    Path *path;
    bool minimized;
    do {
	path = freshPaths.removeHead();
	minimized = tryMinimization(path);
	triedPaths.add(path);
    } while (!minimized || freshPaths.isEmpty());
    path = triedPaths.removeHead();
    // clean up unused paths
    while (!freshPaths.isEmpty()) {
	delete freshPaths.removeHead();
    }
    while (!triedPaths.isEmpty()) {
	delete triedPaths.removeHead();
    }

    // optimize remaining blocks
}

bool Scheduler::isCyclic() const
{
    // Build depth-first tree. If any node A can reach a node B
    // and B lies on the path from A to the root, then and only then
    // the graph contains a strongly connected component (which is
    // equal to the presence of a cycle).
    Q_ASSERT(graph_ != 0);
    DepthFirstMap allNodes;
    BlockList next;
    bool cycleFound = false;
    {
	BlockList blocks = graph_->blocks();
	if (!blocks.isEmpty()) {
	    next.push_front(blocks.first());
	}
    }
    DepthFirstNode *currentNode = 0;
    while (!next.isEmpty() && !cycleFound) {
	// fetch next
	DepthFirstNode *previousNode = currentNode;
	BlockNode *current = next.front();
	next.pop_front();
	// process
	DepthFirstMap::iterator it = allNodes.find(current);
	if (it != allNodes.end()) {
	    // found the node, check if that is a cylce
	    // otherwise ignore edge
	    currentNode = *it;
	    DepthFirstNode *searcher = previousNode;
	    while (!cycleFound && searcher != 0) {
		cycleFound = searcher == currentNode;
		searcher = searcher->prev();
	    }
	}
	else {
	    // new node found
	    currentNode = new DepthFirstNode(current, previousNode);
	    allNodes[current] = currentNode;

	    // push adjacent
	    QPtrList<BlockNode> neighbours = current->neighbours();
	    QPtrListIterator<BlockNode> adjacent(neighbours);
	    while (!adjacent.isEmpty()) {
		next.push_front(adjacent++);
	    }
	}
    }
    DepthFirstMapIterator currentItem;
    for (currentItem = allNodes.begin(); currentItem != allNodes.end();
	 ++currentItem) {

	delete *currentItem;
    }
    return cycleFound;
}
