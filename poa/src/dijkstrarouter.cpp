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
 * $Id: dijkstrarouter.cpp,v 1.2 2003/12/15 04:07:09 keulsn Exp $
 *
 *****************************************************************************/


#include "dijkstrarouter.h"

#include "blockview.h"
#include "connectorviewsegment.h"
#include "genericpriorityqueue.h"
#include "grid.h"

#include <climits>
#include <map>



/********
 * Node *
 ********/

Node::Node(QPoint point, LineDirection dir)
{
    point_ = point;
    dir_ = dir;
}

bool Node::operator<(const Node &other) const
{
    if (point_.x() == other.point_.x()) {
	if (point_.y() == other.point_.y()) {
	    return dir_ < other.dir_;
	}
	else {
	    return point_.y() < other.point_.y();
	}
    }
    else {
	return point_.x() < other.point_.x();
    }
}

bool Node::operator==(const Node &other) const
{
    return dir_ == other.dir_ && point_ == other.point_;
}

QPoint Node::point() const
{
    return point_;
}

LineDirection Node::dir() const
{
    return dir_;
}




/*****************
 * PossiblePoint *
 *****************/

QCanvas *PossiblePoint::canvas_ = 0;


PossiblePoint::PossiblePoint(Node node)
    : node_(node)
{
    prev_ = 0;
    weight_ = UINT_MAX;
    updatePriority();
}


PossiblePoint::PossiblePoint(PossiblePoint *prev, Node node)
    : node_(node)
{
    prev_ = prev;
    Q_ASSERT(prev_ == 0 || isRightAngle(prev_->direction(), direction()));
    if (prev == 0) {
	weight_ = 0;
    }
    else {
	weight_ = UINT_MAX;
    }
    updateWeight();
}


bool PossiblePoint::higherPriority(const PriorityItem *other) const
{
    const PossiblePoint *otherPoint =
	dynamic_cast<const PossiblePoint*>(other);
    Q_ASSERT(otherPoint != 0);
    return otherPoint->weight() >= this->weight();
}

LineDirection PossiblePoint::direction() const
{
    return node_.dir();
}

QPoint PossiblePoint::point() const
{
    return node_.point();
}

unsigned PossiblePoint::weight() const
{
    return weight_;
}

void PossiblePoint::reach(PossiblePoint *prev)
{
    unsigned newWeight = calcWeight(prev);
    if (newWeight < weight_) {
	prev_ = prev;
	weight_ = newWeight;
	updatePriority();
    }
}

PossiblePoint *PossiblePoint::prev() const
{
    return prev_;
}

unsigned PossiblePoint::calcWeight(PossiblePoint *prev) const
{
    return prev->weight() + lineWeight(prev_->point(), point());
}

void PossiblePoint::updateWeight()
{
    if (prev_ != 0) {
	weight_ = prev_->weight() + lineWeight(prev_->point(), point());
	updatePriority();
    }
}


unsigned PossiblePoint::lineWeight(QPoint from, QPoint to) const
{
    unsigned value = 0;
    int left = QMIN(from.x(), to.x());
    int top = QMIN(from.y(), to.y());
    QRect rect = QRect(left,
		       top,
		       QABS(from.x() - to.x()),
		       QABS(from.y() - to.y()));
    if (rect.width() == 1) {
	rect.setLeft(rect.left() - 1);
	rect.setWidth(rect.width() + 1);
    }
    if (rect.height() == 1) {
	rect.setTop(rect.top() - 1);
	rect.setHeight(rect.height() + 1);
    }

    // collect all collisions
    QCanvasItemList newCollisions = PossiblePoint::canvas_->collisions(rect);
    QCanvasItemList::iterator current;
    for (current = newCollisions.begin();
	 current != newCollisions.end(); ++current) {
	
	if (INSTANCEOF(*current, ConnectorViewSegment)) {
	    value += 5000;
	}
	else if (INSTANCEOF(*current, BlockView)) {
	    value += 10000;
	}
    }

    // calculate altogether weight
    return value + 400;
}



/******************
 * DijkstraRouter *
 ******************/

typedef GenericPriorityQueue<PossiblePoint> PossiblePointQueue;
typedef std::map<Node, PossiblePoint*> PossiblePointMap;

void seeNode(Node node, PossiblePoint *predecessor,
	     PossiblePointQueue &unreached, PossiblePointMap& seen)
{
    PossiblePointMap::iterator pos = seen.find(node);
    PossiblePoint *reachedPoint;
    if (pos == seen.end()) {
	// new node, insert
	reachedPoint = new PossiblePoint(predecessor, node);
	seen[node] = reachedPoint;
	unreached.insert(reachedPoint);
    }
    else {
	// node already known, update weight
	reachedPoint = (*pos).second;
	reachedPoint->reach(predecessor);
    }
}

QValueList<QPoint> *DijkstraRouter::routeOne(QPoint from,
					     LineDirection fromDir,
					     QPoint to,
					     LineDirection toDir)
{
    //    return routeConnector(from, fromDir, to, toDir);

    PossiblePointQueue unreached;
    PossiblePointMap seen;

    Grid grid(from, to, 10);
    int x, y;
    grid.getGridDistance(from, to, x, y);
    int maxDist = QMAX(300, 2 * (QABS(x) + QABS(y)));

    Q_ASSERT(fromDir != UNKNOWN);
    Q_ASSERT(toDir != UNKNOWN);
    Node startNode = Node(grid.closestGridPoint(from), fromDir);
    Node endNode = Node(grid.closestGridPoint(to), toDir);
    seeNode(startNode, 0, unreached, seen);
    PossiblePoint *target = new PossiblePoint(endNode);
    seen[endNode] = target;
    unreached.insert(target);

    PossiblePoint *current = unreached.removeHead();
    while (current != target && !unreached.isEmpty()) {
	for (int i = 1; true; ++i) {
	    QPoint newPoint = grid.move(current->point(), 
					current->direction(),
					i);
	    grid.getGridDistance(to, newPoint, x, y);
	    if (QABS(x) + QABS(y) > maxDist) {
	        break;
	    }

	    Node newNode(newPoint, current->direction());
	    if (newNode == endNode) {
		seeNode(newNode, current, unreached, seen);
	    }

	    newNode = Node(newPoint, turnLeft(current->direction()));
	    if (newNode != endNode) {
		// cannot dock after turn!
		seeNode(newNode, current, unreached, seen);
	    }

	    newNode = Node(newPoint, turnRight(current->direction()));
	    if (newNode != endNode) {
		// cannot dock after turn!
		seeNode(newNode, current, unreached, seen);
	    }
	}

	Q_ASSERT(!unreached.isEmpty());
	current = unreached.removeHead();
    }

    // extract point sequence if a minimum weight way was found
    QValueList<QPoint> *result;
    if (current == target) {
	result = new QValueList<QPoint>;
	while (current != 0) {
	    result->prepend(current->point());
	    current = current->prev();
	}
    }
    else {
	result = 0;
    }
    // clean up the mess
    unreached.clear();
    PossiblePointMap::iterator seenIt = seen.begin();
    while (seenIt != seen.end()) {
	delete (*seenIt).second;
	++seenIt;
    }
    return result;
}


void DijkstraRouter::route(ConnectorViewList *view)
{
    // FIX
}

void DijkstraRouter::route(QValueList<ConnectorViewList*>& items)
{
    // FIX
}
