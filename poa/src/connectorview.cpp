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
 * $Id: connectorview.cpp,v 1.10 2003/09/18 06:17:52 garbeam Exp $
 *
 *****************************************************************************/


#include <math.h>

#include "connectorview.h"
#include "grid.h"
#include "pinmodel.h"
#include "blockmodel.h"


ConnectorView::ConnectorView(PinView *from,
			     PinView *to,
			     QCanvas *canvas)
    : QCanvasLine(canvas)
{
    from_ = 0;
    to_ = 0;
    first_ = true;
    last_ = true;
    prev_.pin = 0;
    next_.pin = 0;
    
    /*    QValueList<QPoint> *list = routeConnector(from->connectorPoint(),
					      from->connectorDirection(),
					      true,
					      to->connectorPoint(),
					      to->connectorDirection(),
					      true);*/
    QValueList<QPoint> *list = routeConnector(from->connectorPoint(),
					      from->connectorDirection(),
					      to->connectorPoint(),
					      to->connectorDirection());
    applyPointList(list, to);
}

ConnectorView::ConnectorView(PinView *from,
			     QPoint to,
			     LineDirection toDir,
			     QCanvas *canvas)
    : QCanvasLine(canvas)
{
    from_ = 0;
    to_ = 0;
    first_ = true;
    last_ = true;
    prev_.pin = 0;
    next_.pin = 0;
    
    QValueList<QPoint> *list = /*routeConnector(from->connectorPoint(),
					      from->connectorDirection(),
					      true,
					      to,
					      toDir,
					      false);*/ 0;
    applyPointList(list);
}


ConnectorView::ConnectorView(QPoint first,
			     QPoint second,
			     QCanvas *canvas)
    : QCanvasLine(canvas)
{
    first_ = true;
    prev_.pin = 0;
    last_ = true;
    next_.pin = 0;
    setPoints(first.x(), first.y(), second.x(), second.y());
}

ConnectorView::~ConnectorView()
{
    if (first_ && prev_.pin != 0) {
	// FIX: notify prev_.pin
    }
    if (last_ && next_.pin != 0) {
	// FIX: notify next_.pin
    }
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


QValueList<QPoint> *ConnectorView::routeConnector(QPoint from,
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
	    next = grid.move(next, -fromDir, abs(fromDist) + 2);
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
	    next = grid.move(next, -fromDir, abs(fromDist) + 2);
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
	else if (fromAlternateDist >= 2) { // && fromDist <= 0
	    // U-turn then one bending
	    next = grid.move(from, fromDir, 1);
	    list->append(next);
	    next = grid.move(next, fromAlternateDir, fromAlternateDist / 2);
	    list->append(next);
	    next = grid.move(next, -fromDir, abs(fromDist) + 1);
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
	    next = grid.move(next, -fromDir, abs(fromDist) + 1);
	    list->append(next);
	}
    }
    else if (isTurn(fromDir, toDir)) {
	if (fromAlternateDist == 0) {
	    next = grid.move(from, fromDir, 1);
	    list->append(next);
	    next = grid.move(next, turnLeft(fromDir), 1);
	    list->append(next);
	    next = grid.move(next, fromDir, abs(fromDist));
	    list->append(next);
	    next = grid.move(next, turnRight(fromDir), 1);
	    list->append(next);
	}
	else {
	    // U-turn
	    if (abs(fromDist) >= 0) {
		next = grid.move(from, fromDir, abs(fromDist) + 1);
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


void ConnectorView::applyPointList(QValueList<QPoint> *list,
				   PinView *targetPin)
{
    ConnectorView *current = this;
    QPoint first;
    qDebug("list->front()");
    QPoint second = list->front();
    qDebug("pop_front()");
    list->pop_front();
    qDebug("done.");
    // if only one point is on list, then this point must be used.
    setPoints(second.x(), second.y(), second.x(), second.y());
    while (!list->isEmpty()) {
	first = second;
	second = list->front();
	list->pop_front();
	current->setPoints(first.x(), first.y(), second.x(), second.y());
	if (!list->isEmpty()) {
	    if (!current->last_) {
		current = current->next_.connector;
	    }
	    else {
		ConnectorView *newOne = new ConnectorView(first,
							  second,
							  canvas());
		newOne->setNextPin(current->next_.pin);
		newOne->setPrevConnector(current);
		current->setNextConnector(newOne);
		current = newOne;
	    }
	}
    }
    
    qDebug("destroySuccessors()");
    current->destroySuccessors();
    qDebug("done.");
    if (current->next_.pin != targetPin) {
	current->setNextPin(targetPin);
    }
    qDebug("delete list");
    delete list;
    qDebug("done.");
}

void ConnectorView::destroySuccessors()
{
    if (!this->last_) {
	PinView *targetPin = 0;
	ConnectorView *current = this->next_.connector;
	while (current != 0) {
	    ConnectorView *prev = current;
	    if (!prev->last_) {
		current = prev->next_.connector;
	    }
	    else {
		targetPin = prev->next_.pin;
		current = 0;
	    }
	    delete prev;
	}

	setNextPin(targetPin);
    }
    qDebug("destroySuccessors done.");
}


/*
bool ConnectorView::canUseDir(LineDirection goDir,
			      bool honorGoDir,
			      LineDirection dir)
{
    return (dir != UNKNOWN && (honorGoDir && goDir == dir))
	|| (!honorGoDir && isRightAngle(goDir, dir));
}


QValueList<QPoint> *ConnectorView::routeUsingLastButOne(QPoint startPoint,
							LineDirection startDir,
							bool honorStartDir,
							QPoint lastButOne,
							QPoint endPoint)
{
    LineDirection dir;
    if (lastButOne.x() < endPoint.x()) {
	dir = RIGHT;
	Q_ASSERT(lastButOne.y() == endPoint.y());
    }
    else if (lastButOne.x() > endPoint.x()) {
	dir = LEFT;
	Q_ASSERT(lastButOne.y() == endPoint.y());
    }
    else if (lastButOne.y() < endPoint.y()) {
	dir = DOWN;
	Q_ASSERT(lastButOne.x() == endPoint.x());
    }
    else if (lastButOne.y() > endPoint.y()) {
	dir = UP;
	Q_ASSERT(lastButOne.x() == endPoint.y());
    }
    else {
	Q_ASSERT(false);
    }
    QValueList<QPoint> *list = routeConnector(startPoint, 
					      startDir,
					      honorStartDir,
					      lastButOne,
					      dir,
					      false);
    list->append(endPoint);
    return list;
}

unsigned counter = 0;

QValueList<QPoint> *ConnectorView::routeConnector(QPoint startPoint,
						  LineDirection startDir,
						  bool honorStartDir,
						  QPoint endPoint,
						  LineDirection endDir,
						  bool honorEndDir)
{
    qDebug(QString("routeConnector(QPoint(") +
	   QString::number(startPoint.x()) + ", " +
	   QString::number(startPoint.y()) + "), QPoint(" + 
	   QString::number(endPoint.x()) + ", " +
	   QString::number(endPoint.y()) + "))");
    int x;
    int y;

    Grid grid(startPoint, endPoint, 10);

    LineDirection currentDir = startDir;
    bool honorCurrentDir = honorStartDir;
    QPoint currentPoint = startPoint;

    QValueList<QPoint> *list = new QValueList<QPoint>;
    QValueList<QPoint> *afterList = new QValueList<QPoint>;

    while (currentPoint != endPoint && counter < 30) {

	++counter;
	list->append(currentPoint);
	qDebug("Point = (" + QString::number(currentPoint.x()) + ", " +
	       QString::number(currentPoint.y()) + ")");

	grid.getGridDistance(currentPoint, endPoint, x, y);

	LineDirection direct = UNKNOWN;
	LineDirection primary = UNKNOWN;
	LineDirection secondary = UNKNOWN;

	if (x > 0) {
	    primary = RIGHT;
	    qDebug("primary == RIGHT");
	}
	else if (x < 0) {
	    primary = LEFT;
	    qDebug("primary == LEFT");
	}

	if (y > 0) {
	    secondary = DOWN;
	    qDebug("secondary == DOWN");
	}
	else if (y < 0) {
	    secondary = UP;
	    qDebug("secondary == UP");
	}

	unsigned primaryDistance = abs(x);
	unsigned secondaryDistance = abs(y);

	if (primaryDistance < secondaryDistance) {
	    //	    qDebug("xchange dirs");
	    LineDirection temp = primary;
	    primary = secondary;
	    primary = temp;
	    unsigned distTemp = primaryDistance;
	    primaryDistance = secondaryDistance;
	    secondaryDistance = distTemp;
	}

	qDebug("primaryDistance == " + QString::number(primaryDistance));
	qDebug("secondaryDistance == " + QString::number(secondaryDistance));

	// Check if direct connection using a straight line is possible
	if (secondary == UNKNOWN) {
	    // primary may be UNKNOWN as well. In that case startPoint ==
	    // endPoint
	    direct = primary;
	    //	    qDebug("Using direct connection");
	}
	else {
	    direct = UNKNOWN;
	    //	    qDebug("direct == UNKNOWN");
	}

	// Check if direct connection in a straight line is possible
	bool directPossible = direct != UNKNOWN
	    && canUseDir(currentDir, honorCurrentDir, direct)
	    && canUseDir(endDir, honorEndDir, direct);

	if (directPossible) {
	    qDebug("directPossible");
	}
	else {
	    qDebug("!directPossible");
	}

	bool stepDone = false;
	if (directPossible) {
	    // Direct connection possible
	    afterList->prepend(endPoint);
	    currentPoint = endPoint;
	    stepDone = true;
	}
	else if (honorEndDir) {
	    // No direct connection possible, weaken end direction

	    // Add last segment, if its length predictably == 1
	    switch (endDir) {
	    case RIGHT:
		if (x <= 0 || (x == 1 && (y != 0 || !canUseDir(currentDir,
							       honorCurrentDir,
							       RIGHT)))) {
		    // docking at end point not possible in a straight line
		    afterList->prepend(endPoint);
		    endPoint = grid.move(endPoint, LEFT, 1);
		    honorEndDir = false;
		    stepDone = true;
		}
		break;
	    case LEFT:
		if (x >= 0 || (x == -1 && (y != 0 || !canUseDir(currentDir,
								honorCurrentDir,
								LEFT)))) {
		    // docking at end point not possible in a straight line
		    afterList->prepend(endPoint);
		    endPoint = grid.move(endPoint, RIGHT, 1);
		    honorEndDir = false;
		    stepDone = true;
		}
		break;
	    case DOWN:
		if (y <= 0 || (y == 1 && (x != 0 || !canUseDir(currentDir,
							       honorCurrentDir,
							       DOWN)))) {
		    // docking at end point not possible in a straight line
		    afterList->prepend(endPoint);
		    endPoint = grid.move(endPoint, UP, 1);
		    honorEndDir = false;
		    stepDone = true;
		}
		break;
	    case UP:
		if (y >= 0 || (y == -1 && (x != 0 || !canUseDir(currentDir,
								honorCurrentDir,
								UP)))) {
		    // docking at end point not possible in a straight line
		    afterList->prepend(endPoint);
		    endPoint = grid.move(endPoint, DOWN, 1);
		    honorEndDir = false;
		    stepDone = true;
		}
		break;
	    }
	}

	if (!stepDone) {
	    // No last segment added.
	    // Try to satisfy direct as much as possible trying primary and
	    // secondary directions
	    Q_ASSERT(primary != UNKNOWN);
	    if (!canUseDir(currentDir, honorCurrentDir, primary)) {
		primary = secondary;
		primaryDistance = secondaryDistance;
		secondary = UNKNOWN;
	    }
	    else if (!canUseDir(currentDir, honorCurrentDir, secondary)) {
		secondary = UNKNOWN;
	    }

	    unsigned distance;
	    
	    if (primary == UNKNOWN) {
		// no sensible direction possible, must turn around
		if (honorCurrentDir) {
		    // must go one dummy step to weaken honorCurrentDir
		    primary = currentDir;
		}
		else {
		    // must choose one direction orthogonal to currentDir
		    primary = turnLeft(currentDir);
		}
		distance = 1;
	    }
	    else {
		// Choose a decent direction and distance
		if (isRightAngle (primary, endDir) || !honorEndDir) {
		    distance = primaryDistance;
		}
		else {
		    distance = primaryDistance / 2;
		}
	    }

	    qDebug("next point == " +
	      QString::number(dest.x()) + ", " + QString::number(dest.y()));

	    currentPoint = grid.move(currentPoint, primary, distance);
	    currentDir = primary;
	    honorCurrentDir = false;

	}
    }

    while (!afterList->isEmpty()) {
	list->append(afterList->front());
	afterList->pop_front();
    }
    list->append(endPoint);

    return list;
}*/


QCanvasItemList ConnectorView::allSegments()
{
    QCanvasItemList list;
    ConnectorView *current = this;

    while (current != 0) {
    list.prepend(current);
    if (current->last_) {
        current = 0;
    } else {
        current = current->next_.connector;
    }
    }
    return list;
}


void ConnectorView::setPrevConnector(ConnectorView *prev)
{
    first_ = prev == 0;
    prev_.connector = prev;
}

void ConnectorView::setNextConnector(ConnectorView *next)
{
    last_ = next == 0;
    next_.connector = next;
}

void ConnectorView::setPrevPin(PinView *source)
{
    first_ = true;
    prev_.pin = source;
}

void ConnectorView::setNextPin(PinView *target)
{
    last_ = true;
    next_.pin = target;
}

QString ConnectorView::tip()
{
    return QString("<b>Connector</b><br><hr>" \
                   "<b>Source:</b> <u>%1</u>::%2<br>" \
                   "<b>Target:</b> <u>%3</u>::%4<br>" \
                   "<b>Width:</b> %5")
        .arg(from()->pinModel()->name())
        .arg(from()->pinModel()->parent()->name())
        .arg(to()->pinModel()->name())
        .arg(to()->pinModel()->parent()->name())
        .arg(to()->pinModel()->bits());
}


PinView *ConnectorView::to()
{
    return to_;
}


PinView *ConnectorView::from()
{
    return from_;
}
