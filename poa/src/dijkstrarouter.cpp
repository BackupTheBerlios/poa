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
 * $Id: dijkstrarouter.cpp,v 1.4 2004/01/09 16:09:08 keulsn Exp $
 *
 *****************************************************************************/


#include "dijkstrarouter.h"

#include "blockview.h"
#include "connectorviewlist.h"
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

bool Node::operator!=(const Node &other) const
{
    return !(dir_ == other.dir_ && point_ == other.point_);
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
	// first point
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
    return this->weight() < otherPoint->weight();
}

Node PossiblePoint::node() const
{
    return node_;
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
    Q_ASSERT(prev != 0);
    return prev->weight() + lineWeight(prev->point(), point());
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
    int left = QMIN(from.x(), to.x());
    int top = QMIN(from.y(), to.y());
    unsigned width = QABS(from.x() - to.x());
    unsigned height = QABS(from.y() - to.y());

    unsigned value = width + height;

    QRect rect = QRect(left,
		       top,
		       width,
		       height);
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
	
	QCanvasRectangle *blockTest =
	    dynamic_cast<QCanvasRectangle*>(*current);

	if (blockTest != 0) {
	    value += 10000;
	} else {
	    qDebug("5000");
	    value += 5000;
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
	// node already known, update weight"
	reachedPoint = (*pos).second;
	Q_ASSERT(reachedPoint != 0);
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
    int minX = QMIN(from.x(), to.x()) - 30;
    int minY = QMIN(from.y(), to.y()) - 30;
    int maxX = QMAX(from.x(), to.x()) + 30;
    int maxY = QMAX(from.y(), to.y()) + 30;

    Q_ASSERT(fromDir != UNKNOWN);
    Q_ASSERT(toDir != UNKNOWN);
    Node startNode = Node(grid.closestGridPoint(from), fromDir);
    Node endNode = Node(grid.closestGridPoint(to), toDir);
    seeNode(startNode, 0, unreached, seen);
    PossiblePoint *target = new PossiblePoint(endNode);
    seen[endNode] = target;
    unreached.insert(target);

    Q_ASSERT(unreached.size() == 2);
    PossiblePoint *current = unreached.removeHead();
    Q_ASSERT(current->node() == startNode);
    while (current != target && !unreached.isEmpty()) {
	for (int i = 1; true; ++i) {
	    QString errors = unreached.checkIntegrity();
	    if (errors != QString::null) {
		qDebug(errors);
	    }

	    QPoint newPoint = grid.move(current->point(), 
					current->direction(),
					i);
	    grid.getGridDistance(to, newPoint, x, y);
	    //qDebug("(" + QString::number(x) + ", " + QString::number(y)
	    //	   + "), " + image(current->direction()));
	    if (newPoint.x() < minX || newPoint.y() < minY
		|| newPoint.x() > maxX || newPoint.y() > maxY) {

	        break;
	    }

	    Node newNode(newPoint, current->direction());
	    if (newNode == endNode) {
		qDebug("endNode seen");
		seeNode(newNode, current, unreached, seen);
		qDebug("done seeing.");
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

    qDebug("target reached");
    // extract point sequence if a minimum weight way was found
    QValueList<QPoint> *result;
    if (current == target) {
	result = new QValueList<QPoint>;
	while (current != 0) {
	    qDebug(image(current->point()));
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
    PossiblePoint::canvas_ = view->canvas();

    PinView *source = view->source();
    PinView *target = view->target();
    QValueList<QPoint> *points = routeOne(source->connectorPoint(),
					  source->connectorSourceDir(),
					  target->connectorPoint(),
					  target->connectorTargetDir());
    if (points != 0) {
	view->applyPointList(*points);
	delete points;
    }
}

void DijkstraRouter::route(QValueList<ConnectorViewList*>& items)
{
    QValueList<ConnectorViewList*>::iterator it;
    for (it = items.begin(); it != items.end(); ++it) {
	route(*it);
    }
}
