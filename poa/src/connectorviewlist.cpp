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
 * $Id: connectorviewlist.cpp,v 1.14 2003/09/26 22:00:38 garbeam Exp $
 *
 *****************************************************************************/


#include "connectorviewlist.h"

#include "blockmodel.h"
#include "blockview.h"
#include "connectorviewsegment.h"
#include "grid.h"
#include "gridcanvas.h"
#include "pinmodel.h"
#include "reachedpoint.h"

#include <qcanvas.h>
#include <qpoint.h>
#include <qptrdict.h>
#include <qvaluelist.h>

#include <queue>
#include <vector>


// std added because of gcc 3.2 issues
typedef std::priority_queue<ReachedPoint,
               std::vector<ReachedPoint>,
		       CompareReachedPoints> ReachedQueue;


QString image(QPoint p)
{
    return "(" + QString::number(p.x()) + ", " + QString::number(p.y()) + ")";
}

ConnectorViewList::ConnectorViewList(PinView *source,
                                     PinView *target,
                                     GridCanvas *canvas,
                                     QDomElement *element)
{
    source_ = source;
    target_ = target;
    canvas_ = canvas;
    Q_ASSERT(source->canvas() == canvas_);
    Q_ASSERT(target->canvas() == canvas_);

    connect(source->pinModel(), SIGNAL(deleted()),
            this, SLOT(deleteView()));
    connect(target->pinModel(), SIGNAL(deleted()),
            this, SLOT(deleteView()));
    connect(source, SIGNAL(moved(PinView*)),
            this, SLOT(pinMoved(PinView*)));
    connect(target, SIGNAL(moved(PinView*)),
            this, SLOT(pinMoved(PinView*)));

    if (element == 0) {
        QValueList<QPoint> *points =
            routeConnector(source->connectorPoint(),
                           source->connectorSourceDir(),
                           target->connectorPoint(),
                           target->connectorTargetDir());
        applyPointList(*points);
        delete points;
    }
    else {
        deserialize(element);
    }
}

ConnectorViewList::~ConnectorViewList()
{
    if (source_ != 0 && source_->pinModel() != 0) {
        source_->pinModel()->detach();
    }
    if (target_ != 0 && target_->pinModel() != 0) {
        target_->pinModel()->detach();
    }
    deleteAllConnectorViews();
}

PinView *ConnectorViewList::source()
{
    return source_;
}

PinView *ConnectorViewList::target()
{
    return target_;
}

const QCanvasItemList ConnectorViewList::allSegments()
{
    return segments_;
}

void ConnectorViewList::deleteSegment(ConnectorViewSegment *seg)
{
    Q_ASSERT(segments_.contains(seg));
    segments_.remove(seg);
    delete seg;
}

QValueList<QPoint> ConnectorViewList::points()
{
    QValueList<QPoint> list;
    QValueList<QPoint>::iterator point = list.end();
    QCanvasItemList::iterator segment = segments_.begin();
    while (segment != segments_.end()) {
        ConnectorViewSegment *current = (ConnectorViewSegment*) *segment;
        list.insert(point, current->startPoint());
        ++point;
        ++segment;
    }
    if (!segments_.isEmpty()) {
        ConnectorViewSegment *last = (ConnectorViewSegment*) segments_.last();
        list.insert(point, last->endPoint());
    }
    return list;
}

QString ConnectorViewList::tip()
{
    return QString("<b>Connector</b><br><hr>" \
                   "<b>Source:</b> <u>%1</u>::%2<br>" \
                   "<b>Target:</b> <u>%3</u>::%4<br>" \
                   "<b>Width:</b> %5")
        .arg(source()->pinModel()->name())
        .arg(source()->pinModel()->parent()->name())
        .arg(target()->pinModel()->name())
        .arg(target()->pinModel()->parent()->name())
        .arg((unsigned)target()->pinModel()->bits());
}

void ConnectorViewList::setSelected(bool selected)
{
    emit select(selected);
}

QDomElement ConnectorViewList::serialize(QDomDocument *document)
{
    QDomElement root = document->createElement("connector-view");
    QDomElement points = document->createElement("points");
    root.appendChild(points);
    root.setAttribute("source-block", source_->pinModel()->parent()->id());
    root.setAttribute("source-pin", source_->pinModel()->id());
    root.setAttribute("target-block", target_->pinModel()->parent()->id());
    root.setAttribute("target-pin", target_->pinModel()->id());
    QValueList<QPoint> pList = ConnectorViewList::points();
    QValueList<QPoint>::Iterator it;
    for( it = pList.begin(); it != pList.end(); ++it ) {
        QDomElement point = document->createElement("point");
        point.setAttribute("x", (*it).x());
        point.setAttribute("y", (*it).y());
        points.appendChild(point);
    }
    return root;
}

void ConnectorViewList::deserialize(QDomElement *element)
{
    QDomNodeList pList = element->elementsByTagName("point");
    QValueList<QPoint> *points = new QValueList<QPoint>;
    for (uint i = 0; i < pList.count(); i++) {
        QDomElement pEl = pList.item(i).toElement();
        points->append(QPoint(pEl.attribute("x","0").toUInt(),
                             pEl.attribute("y","0").toUInt()));
    }
    applyPointList(*points);
    delete points;
}

void ConnectorViewList::applyPointList(const QValueList<QPoint> &list)
{
    Q_ASSERT(list.size() >= 2);

    QValueList<QPoint>::const_iterator point = list.begin();
    QValueListIterator<QCanvasItem*> it = segments_.begin();

    QPoint second = *point;
    ++point;
    QPoint first;

    while (point != list.end()) {
        first = second;
        second = *point;
        ++point;

        ConnectorViewSegment *current;
        if (it != segments_.end()) {
            current = (ConnectorViewSegment*) *it;
            current->setPoints(first.x(), first.y(), second.x(), second.y());
        }
        else {
            current = new ConnectorViewSegment(first, second, canvas_, this);
            connect(this, SIGNAL(select(bool)), current, SLOT(select(bool)));
            it = segments_.insert(it, current);
            current->show();
        }
        ++it;
    }

    while (it != segments_.end()) {
        ConnectorViewSegment *current = (ConnectorViewSegment*) *it;
        it = segments_.erase(it);
        delete current;
    }
}

void ConnectorViewList::deleteAllConnectorViews()
{
    QValueListIterator<QCanvasItem*> it = segments_.begin();
    while (it != segments_.end()) {
        QCanvasItem *view = *it;
        delete view;
        it = segments_.erase(it);
    }
}

void ConnectorViewList::pinMoved(PinView *pin)
{
    if (pin == source_ || pin == target_) {
	Q_ASSERT(pin->canvas() == canvas_);

        QValueList<QPoint> *points =
          routeConnector(source_->connectorPoint(),
                         source_->connectorSourceDir(),
                         target_->connectorPoint(),
                         target_->connectorTargetDir());

        applyPointList(*points);
        delete points;
    }
}

void ConnectorViewList::deleteView()
{
    delete this;
}


LineDirection operator-(LineDirection dir)
{
    switch (dir) {
    case LEFT:
        return RIGHT;
    case RIGHT:
        return LEFT;
    case UP:
        return DOWN;
    case DOWN:
        return UP;
    case UNKNOWN:
        return UNKNOWN;
    default:
        Q_ASSERT(false);
        return UNKNOWN;
    }
}

int distInDir(LineDirection dir, int x, int y)
{
    switch (dir) {
    case LEFT:
        return -x;
    case RIGHT:
        return x;
    case UP:
        return -y;
    case DOWN:
        return y;
    default:
        Q_ASSERT(false);
        return 0;
    }
}

LineDirection alternateDir(LineDirection dir, int x, int y)
{
    switch (dir) {
    case LEFT:
    case RIGHT:
        if (y > 0) {
            return DOWN;
        }
        else {
            return UP;
        }
    case UP:
    case DOWN:
        if (x > 0) {
            return RIGHT;
        }
        else {
            return LEFT;
        }
    default:
        Q_ASSERT(false);
        return UNKNOWN;
    }
}


bool isRightAngle(LineDirection first, LineDirection second)
{
    switch (first) {
    case LEFT:
    case RIGHT:
        return second == UP || second == DOWN;
    case UP:
    case DOWN:
        return second == LEFT || second == RIGHT;
    case UNKNOWN:
        return false;
    default:
        Q_ASSERT(false);
        return false;
    }
}

bool isTurn(LineDirection first, LineDirection second)
{
  Q_ASSERT(first != UNKNOWN && second != UNKNOWN);
  return first == LEFT && second == LEFT
    || first == RIGHT && second == RIGHT
    || first == UP && second == UP
    || first == DOWN && second == DOWN;
}

LineDirection turnLeft(LineDirection dir)
{
    switch (dir) {
    case LEFT:
        return DOWN;
    case DOWN:
        return RIGHT;
    case RIGHT:
        return UP;
    case UP:
        return LEFT;
    default:
        Q_ASSERT(false);
        return UNKNOWN;
    }
}

LineDirection turnRight(LineDirection dir)
{
    return -turnLeft(dir);
}

QValueList<QPoint> *ConnectorViewList::routeConnector(QPoint from,
                                                      LineDirection fromDir,
                                                      QPoint to,
                                                      LineDirection toDir)
{
    Q_ASSERT(fromDir != UNKNOWN);

    int x;
    int y;
    Grid grid(from, to, 10);
    grid.getGridDistance(from, to, x, y);
    Q_ASSERT(grid.move(from, x, y) == to);
    //qDebug("from == " + image(from));
    //qDebug("to   == " + image(to));
    //qDebug("closestGridPoint(from) == " +
    //       image(grid.closestGridPoint(from)));
    //qDebug("closestGridPoint(to)   == " + image(grid.closestGridPoint(to)));
    //qDebug("move(from, " + QString::number(x) + ", " +
    //       QString::number(y) + " ) == " + image(grid.move(from, x, y)));
    //qDebug("move((0, 0), 1, 1) == " + image(grid.move(QPoint(0, 0), 1, 1)));

    int fromDist = distInDir(fromDir, x, y);
    LineDirection fromAlternateDir = alternateDir(fromDir, x, y);
    int fromAlternateDist = distInDir(fromAlternateDir, x, y);
    Q_ASSERT(fromAlternateDist >= 0);

    QValueList<QPoint> *list = new QValueList<QPoint>;
    list->append(from);

    QPoint next = from;

    if (toDir == UNKNOWN) {
        // FIX: implementation missing
        Q_ASSERT(false);
    }
    else if (fromDir == toDir) {
        if (fromDist >= 0 && fromAlternateDist == 0) {
            // straight line, already done.
        }
        else if (fromDist >= 2 && fromAlternateDist >= 1) {
            // one stair
            next = grid.move(from, fromDir, fromDist / 2);
            list->append(next);
            next = grid.move(next, fromAlternateDir, fromAlternateDist);
            list->append(next);
        }
        else if (fromDist < 2 && fromAlternateDist >= 1) {
            // S-shape
            next = grid.move(from, fromDir, 1);
            list->append(next);
            next = grid.move(next, fromAlternateDir, fromAlternateDist / 2);
            list->append(next);
            next = grid.move(next, -fromDir, QABS(fromDist) + 2);
            list->append(next);
            next = grid.move(next, fromAlternateDir,
                             (fromAlternateDist + 1) / 2);
            list->append(next);
        }
        else { // fromDist < 0 && fromAlternateDist == 0
            // circle
            next = grid.move(from, fromDir, 1);
            list->append(next);
            next = grid.move(next, turnLeft(fromDir), 1);
            list->append(next);
            next = grid.move(next, -fromDir, QABS(fromDist) + 2);
            list->append(next);
            next = grid.move(next, turnRight(fromDir), 1);
            list->append(next);
        }
    }
    else if (isRightAngle(fromDir, toDir)) {
        if (fromDist >= 1) {
            // one bending
            next = grid.move(from, fromDir, fromDist);
            list->append(next);
        }
        else if (fromAlternateDist >= 2 && toDir == fromAlternateDir) {
                  // && fromDist <= 0
            // U-turn then one bending
            next = grid.move(from, fromDir, 1);
            list->append(next);
            next = grid.move(next, fromAlternateDir, fromAlternateDist / 2);
            list->append(next);
            next = grid.move(next, -fromDir, QABS(fromDist) + 1);
            list->append(next);
        }
        else if (fromAlternateDist >= 2) {
                 // && isTurn (toDir, fromAlternateDir)
                 // && fromDist <=0
            // large enough U-turn then one bending
            next = grid.move(from, fromDir, 1);
            list->append(next);
            next = grid.move(next, fromAlternateDir, fromAlternateDist + 1);
            list->append(next);
            next = grid.move(next, -fromDir, QABS(fromDist) + 1);
            list->append(next);
        }
        else { // 0 <= fromAlternateDist <= 1 && fromDist <= 0
            // spiral
            LineDirection dir;
            if (fromAlternateDir != UNKNOWN) {
                dir = -fromAlternateDir;
            }
            else {
                dir = turnLeft(fromDir);
            }
            next = grid.move(from, fromDir, 1);
            list->append(next);
            next = grid.move(next, dir, 1);
            list->append(next);
            next = grid.move(next, -fromDir, QABS(fromDist) + 1);
            list->append(next);
        }
    }
    else if (isTurn(fromDir, toDir)) {
        if (fromAlternateDist == 0) {
            next = grid.move(from, fromDir, 1);
            list->append(next);
            next = grid.move(next, turnLeft(fromDir), 1);
            list->append(next);
            next = grid.move(next, fromDir, QABS(fromDist));
            list->append(next);
            next = grid.move(next, turnRight(fromDir), 1);
            list->append(next);
        }
        else {
            // U-turn
            if (fromDist >= 0) {
                next = grid.move(from, fromDir, QABS(fromDist) + 1);
            }
            else {
                next = grid.move(from, fromDir, 1);
            }
            list->append(next);
            next = grid.move(next, fromAlternateDir, fromAlternateDist);
            list->append(next);
        }
    }

    list->append(to);
    return list;
}


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

QValueList<QPoint> *ConnectorViewList::routeOptimizing(QPoint from,
						       LineDirection fromDir,
						       QPoint to,
						       LineDirection toDir)
{
    return routeConnector(from, fromDir, to, toDir);

    
}
