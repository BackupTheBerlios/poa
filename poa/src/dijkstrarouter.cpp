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
 * $Id: dijkstrarouter.cpp,v 1.9 2004/02/19 16:09:10 keulsn Exp $
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

PossiblePoint::PossiblePoint(Node node)
    : node_(node)
{
    prev_ = 0;
    weight_ = UINT_MAX;
    updatePriority();
}


PossiblePoint::PossiblePoint(PossiblePoint *prev,
                             Node node,
                             unsigned weight)
    : node_(node)
{
    prev_ = prev;
    Q_ASSERT(prev_ == 0 || isRightAngle(prev_->direction(), direction()));
    weight_ = weight;
    updatePriority();
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

void PossiblePoint::reach(PossiblePoint *prev, unsigned newWeight)
{
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

unsigned PossiblePoint::calcWeight(PossiblePoint *prev,
                                   QPoint next,
                                   Weighting *weighting)
{
    return prev->weight() +
        weighting->weight(prev->point(), next) + 200;
}


/*************
 * Weighting *
 *************/

const int Weighting::fringe_ = 20;

Weighting::Weighting(QPoint from, QPoint to, QCanvas *canvas):
    grid_(from, 10) // dummy initialization, will be overwritten
{
    Q_ASSERT(Weighting::fringe_ >= 0);
    int xMin = QMIN(from.x(), to.x());
    int yMin = QMIN(from.y(), to.y());
    QPoint topLeft = QPoint(xMin, yMin);
    int xMax = QMAX(from.x(), to.x());
    int yMax = QMAX(from.y(), to.y());
    QPoint bottomRight = QPoint(xMax, yMax);
    grid_ = Grid(topLeft, bottomRight, 10);

    // enlargen weighting area by fringe
    topLeft = grid_.move(topLeft, -Weighting::fringe_, -Weighting::fringe_);
    bottomRight = grid_.move(bottomRight, Weighting::fringe_,
                             Weighting::fringe_);

    // size in grid steps
    int width, height;
    grid_.getGridDistance(topLeft, bottomRight, width, height);
    ++width;
    ++height;
    rect_ = QRect(0, 0, width, height);
    origin_ = topLeft;

    int x, y;
    weighting_ = new unsigned*[height];
    for (y = 0; y < height; ++y) {
        weighting_[y] = new unsigned[width];
        for (x = 0; x < width; ++x) {
            weighting_[y][x] = 1;
        }
    }

    // suppose all canvas items are rectangular
    QCanvasItemList items = canvas->allItems();
    for (QCanvasItemList::const_iterator current = items.begin();
         current != items.end(); ++current) {

        ConnectorViewSegment *currentSegment =
            dynamic_cast<ConnectorViewSegment*>(*current);
        if (currentSegment != 0) {
            ConnectorViewList *currentViewList = currentSegment->viewList();
            if (currentViewList != 0 && currentViewList->awaitsRerouting()) {
                continue;
            }
        }

        QRect bounding = (*current)->boundingRect();
        int left;
        int top;
        int right;
        int bottom;
        topLeft = grid_.closestGridPoint(bounding.topLeft());
        grid_.getGridDistance(origin_, topLeft, left, top);
        bottomRight = grid_.closestGridPoint(bounding.bottomRight());
        grid_.getGridDistance(origin_, bottomRight, right, bottom);
        QRect cover = rect_.intersect(QRect(left,
                                            top,
                                            right - left + 1,
                                            bottom - top + 1));
        for (y = cover.top(); y <= cover.bottom(); ++y) {
            for (x = cover.left(); x <= cover.right(); ++x) {
                weighting_[y - rect_.top()][x - rect_.left()] += 1000;
            }
        }
    }

    //    for (y = rect_.top(); y <= rect_.bottom(); ++y) {
    //  qDebug("y = " + QString::number(y));
    //  for (x = rect_.left(); x <= rect_.right(); ++x) {
    //      qDebug(QString::number(weighting_[y][x]));
    //  }
    //    }
}

Weighting::~Weighting()
{
    for (int y = 0; y < rect_.height(); ++y) {
        delete [] (weighting_[y]);
    }
    delete [] weighting_;
}

QRect Weighting::rect() const
{
    return rect_;
}

QPoint Weighting::gridPoint(QPoint point) const
{
    point = grid_.closestGridPoint(point);
    int x, y;
    grid_.getGridDistance(origin_, point, x, y);
    return QPoint(x, y);
}

QPoint Weighting::translate(QPoint gridPoint) const
{
    return grid_.move(origin_, gridPoint.x(), gridPoint.y());
}

QPoint Weighting::move(QPoint gridPoint, LineDirection dir, int distance) const
{
    switch (dir) {
    case LEFT:
        return QPoint(gridPoint.x() - distance, gridPoint.y());
    case RIGHT:
        return QPoint(gridPoint.x() + distance, gridPoint.y());
    case UP:
        return QPoint(gridPoint.x(), gridPoint.y() - distance);
    case DOWN:
        return QPoint(gridPoint.x(), gridPoint.y() + distance);
    default:
        Q_ASSERT(false);
        return QPoint(0, 0);
    }
}

unsigned Weighting::weight(QPoint from, QPoint to)
{
    unsigned value = 0;
    int xMin = QMIN(from.x(), to.x());
    int yMin = QMIN(from.y(), to.y());
    int width = QABS(from.x() - to.x()) + 1;
    int height = QABS(from.y() - to.y()) + 1;
    QRect rect(xMin, yMin, width, height);
    for (int y = rect.top(); y <= rect.bottom(); ++y) {
        for (int x = rect.left(); x <= rect.right(); ++x) {
            value += weighting_[y][x];
        }
    }
    return value;
}

unsigned Weighting::weightAt(QPoint at)
{
    return weighting_[at.y()][at.x()];
}



/******************
 * DijkstraRouter *
 ******************/

typedef GenericPriorityQueue<PossiblePoint> PossiblePointQueue;
typedef std::map<Node, PossiblePoint*> PossiblePointMap;

void seeNode(Node node, PossiblePoint *predecessor,
             PossiblePointQueue &unreached, PossiblePointMap &seen,
             unsigned weight)
{
    PossiblePointMap::iterator pos = seen.find(node);
    PossiblePoint *reachedPoint;
    if (pos == seen.end()) {
        reachedPoint = new PossiblePoint(predecessor, node, weight);
        seen[node] = reachedPoint;
        unreached.insert(reachedPoint);
    }
    else {
        reachedPoint = (*pos).second;
        Q_ASSERT(reachedPoint != 0);
        reachedPoint->reach(predecessor, weight);
    }
}


QValueList<QPoint> *DijkstraRouter::routeOne(QPoint from,
                                             LineDirection fromDir,
                                             QPoint to,
                                             LineDirection toDir,
                                             QCanvas *canvas)
{
    PossiblePointQueue unreached;
    PossiblePointMap seen;

    Weighting weighting(from, to, canvas);
    QRect searchRect = weighting.rect();

    Node startNode(weighting.gridPoint(from), fromDir);
    Node endNode(weighting.gridPoint(to), toDir);
    seeNode(startNode, 0, unreached, seen, 0);
    PossiblePoint *target = new PossiblePoint(endNode);
    seen[endNode] = target;
    unreached.insert(target);

    Q_ASSERT(unreached.size() == 2);
    PossiblePoint *current = unreached.removeHead();
    Q_ASSERT(current->node() == startNode);
    while (current != target && !unreached.isEmpty()) {
        unsigned currentWeight = current->weight() + 200;
        for (int i = 1; true; ++i) {
            // Q_ASSERT(unreached.checkIntegrity() == QString::null);

            QPoint newPoint = weighting.move(current->point(),
                                             current->direction(),
                                             i);
            if (!searchRect.contains(newPoint)) {
                break;
            }
            currentWeight += weighting.weightAt(newPoint);
            if (currentWeight > target->weight()) {
                break;
            }

            Node newNode(newPoint, current->direction());
            if (newNode == endNode) {
                //qDebug("target seen: " + QString::number(target->weight()));
                seeNode(newNode, current, unreached, seen, currentWeight);
                //qDebug("update:      " + QString::number(target->weight()));
            }

            newNode = Node(newPoint, turnLeft(current->direction()));
            if (newNode != endNode) {
                // cannot dock after turn!
                seeNode(newNode, current, unreached, seen, currentWeight);
            }

            newNode = Node(newPoint, turnRight(current->direction()));
            if (newNode != endNode) {
                // cannot dock after turn!
                seeNode(newNode, current, unreached, seen, currentWeight);
            }
        }

        // Q_ASSERT(!unreached.isEmpty());
        current = unreached.removeHead();
    }

    //qDebug("target reached");
    // extract point sequence if a minimum weight way was found
    QValueList<QPoint> *result;
    // since we insert target into the unreached queue, the case might arise
    // where we find target, but there is no path leading to target.
    // thus we check for this situation by testing if current has a predecessor
    if (current == target && current->prev() != 0) {
        result = new QValueList<QPoint>;
        result->prepend(to);

        //qDebug(image(current->point()) + ": "
        //       + QString::number(current->weight()));

        current = current->prev();
        while (current->prev() != 0) {
            //qDebug(image(current->point()));
            result->prepend(weighting.translate(current->point()));

            //    qDebug(image(current->point())
            //     + ": " + QString::number(current->weight()));

            current = current->prev();
        }


        //qDebug(image(current->point()) + ": " +
        //       QString::number(current->weight()));

        result->prepend(from);
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
    QCanvas *canvas = view->canvas();

    PinView *source = view->source();
    PinView *target = view->target();
    view->awaitRerouting(true);
    QValueList<QPoint> *points = routeOne(source->connectorPoint(),
                                          source->connectorSourceDir(),
                                          target->connectorPoint(),
                                          target->connectorTargetDir(),
                                          canvas);
    if (points != 0) {
        view->applyPointList(*points);
        Q_ASSERT(!view->awaitsRerouting());
        delete points;
    }
}

