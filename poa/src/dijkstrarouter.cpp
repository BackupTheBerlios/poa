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
 * $Id: dijkstrarouter.cpp,v 1.1 2003/12/03 11:13:13 keulsn Exp $
 *
 *****************************************************************************/


#include "dijkstrarouter.h"

#include <queue>


// std added because of gcc 3.2 issues
typedef std::priority_queue<ReachedPoint,
                            std::vector<ReachedPoint>,
                            CompareReachedPoints> ReachedQueue;

unsigned ConnectorViewList::weight(const QValueList<QPoint> &points)
{
    Q_ASSERT(points.size() >= 2);
    QValueList<QPoint>::const_iterator it = points.begin();
    QPoint second = *it;
    ++it;
    QPoint first;
    QPtrDict<unsigned> collisions;
    collisions.setAutoDelete(true);

    while (it != points.end()) {
	first = second;
	second = *it;
	++it;

	int left = QMIN(first.x(), second.x());
	int top = QMIN(first.y(), second.y());
	QRect rect = QRect(left,
			   top,
			   QABS(first.x() - second.x()),
			   QABS(first.y() - second.y()));
	if (rect.width() == 1) {
	    rect.setLeft(rect.left() - 1);
	    rect.setWidth(rect.width() + 1);
	}
	if (rect.height() == 1) {
	    rect.setTop(rect.top() - 1);
	    rect.setHeight(rect.height() + 1);
	}

	// collect all collisions
	QCanvasItemList newCollisions = canvas_->collisions(rect);
	QCanvasItemList::const_iterator current;
	for (current = newCollisions.begin();
	       current != newCollisions.end(); ++current) {

	    unsigned *value = new unsigned;
	    if (INSTANCEOF(*current, ConnectorViewSegment)) {
		*value += 5000;
	    }
	    else if (INSTANCEOF(*current, BlockView)) {
		*value += 10000;
	    }
	    // current is inserted only once. If the same current is
	    // intersected by a different rect then the value will be
	    // updated but not inserted a second time.
	    collisions.insert(*current, value);
	}
    }

    // sum up collision weight
    unsigned collisionWeight = 0;
    QPtrDictIterator<unsigned> curr(collisions);
    for (unsigned *current = curr.current(); current != 0; current = ++curr) {
        collisionWeight += *current;
    }

    // calculate altogether weight
    return collisionWeight + 400 * (points.size() - 1);
}


unsigned ConnectorViewList::lineWeight(QPoint from, QPoint to)
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
    QCanvasItemList newCollisions = canvas_->collisions(rect);
    QCanvasItemList::const_iterator current;
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

QValueList<QPoint> *DijkstraRouter::routeOne(QPoint from,
					     LineDirection fromDir,
					     QPoint to,
					     LineDirection toDir)
{
    //    return routeConnector(from, fromDir, to, toDir);

    ReachedPointQueue unreached;
    ReachedPointMap seen;

    void seeNode(Node node, unsigned badness, ReachedPoint *predecessor)
	{
	    ReachedPointMap::iterator pos = seen.find(node);
	    ReachedPoint *reachedPoint;
	    if (pos == seen.end()) {
		// new node, insert
		reachedPoint = new ReachedPoint(node, badness, predecessor);
		seen[node] = reachedPoint;
		unreached.push(reachedPoint);
	    }
	    else {
		// node already known, update weight?
		reachedPoint = seen[node];
		if (badness < reachedPoint->badness()) {
		    reachedPoint->setBadness(badness);
		    reachedPoint->setPredecessor(predecessor);
		    // FIX: update position in priority queue, not possible
		    // with STL-interface.
		}
	    }

	}

    ReachedPoint *found = 0;

    Q_ASSERT(fromDir != UNKNOWN);
    Q_ASSERT(toDir != UNKNOWN);
    Node startNode = Node(from, fromDir);
    Node endNode = Node(to, toDir);
    seeNode(startNode, 0, 0);
    
    while (found == 0 && !unreached.isEmpty() {
	ReachedPoint *current = unreached.top();
	unreached.pop();
	for (int i = 1; !found && i < /*FIX*/; ++i) {
	    QPoint newPoint = grid.move(current.point(), current.dir(), i);
	    // FIX: optimization possible, incremental search for collisions.
	    unsigned bad = current->badness() + lineWeight(current.point(),
							   newPoint);

	    Node newNode(newPoint, current.dir());
	    if (newNode == endNode) {
		found = newNode;
		seeNode(newNode, bad);
		// need not goon in same direction, unless end node is found.
	    }

	    newNode = Node(newPoint, turnLeft(current.dir()));
	    if (newNode == endNode) {
		found = newNode;
	    }
	    seeNode(newNode, bad);

	    newNode = Node(newPoint, turnRight(current.dir()));
	    if (newNode == endNode) {
		found = newNode;
	    }
	    seeNode(newNode, bad);
	}
    }

    // extract point sequence if a minimum weight way was found
    QValueList<QPoint> *result;
    if (found) {
	result = new QValueList<QPoint>;
	while (found != 0) {
	    result->prepend(found->point());
	    found = found.predecessor();
	}
    }
    else {
	result = 0;
    }
    // clean up the mess
    ReachedPointMap::iterator seenIt = seen.begin();
    while (seenIt != seen.end()) {
	delete (*seenIt).second;
	++seenIt;
    }
    // since 'unreached' only stores aliases to 'seen', we're done.
    return result;
}
