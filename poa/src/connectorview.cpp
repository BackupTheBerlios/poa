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
 * $Id: connectorview.cpp,v 1.13 2003/09/18 14:21:04 keulsn Exp $
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
    from_ = from;
    to_ = to;
    connect(from->pinModel(), SIGNAL(deleted()), this, SLOT(deleteView()));
    connect(to->pinModel(), SIGNAL(deleted()), this, SLOT(deleteView()));
    first_ = true;
    last_ = true;
    prev_.pin = 0;
    next_.pin = 0;

    QValueList<QPoint> *list = routeConnector(from->connectorPoint(),
					      from->connectorDirection(),
					      to->connectorPoint(),
					      to->connectorDirection());
    applyPointList(list, to);
    setPen(QPen(Qt::black, 2));

}

ConnectorView::ConnectorView(PinView *source, PinView *target,
                 QPoint to,
                 LineDirection toDir,
                 QCanvas *canvas)
    : QCanvasLine(canvas)
{
    from_ = source;
    to_ = target;
    connect(source->pinModel(), SIGNAL(deleted()), this, SLOT(deleteView()));
    connect(target->pinModel(), SIGNAL(deleted()), this, SLOT(deleteView()));
    first_ = true;
    last_ = true;
    prev_.pin = 0;
    next_.pin = 0;

    QValueList<QPoint> *list = /*FIX: missing*/ 0;
    applyPointList(list);
    setPen(QPen(Qt::black, 2));

}


ConnectorView::ConnectorView(PinView *source, PinView *target,
                 QPoint first,
                 QPoint second,
                 QCanvas *canvas)
    : QCanvasLine(canvas)
{
    from_ = source;
    to_ = target;
    connect(source->pinModel(), SIGNAL(deleted()), this, SLOT(deleteView()));
    connect(target->pinModel(), SIGNAL(deleted()), this, SLOT(deleteView()));
    first_ = true;
    prev_.pin = 0;
    last_ = true;
    next_.pin = 0;
    setPoints(first.x(), first.y(), second.x(), second.y());
    setPen(QPen(Qt::black, 2));
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

QString image(QPoint p)
{
    return "(" + QString::number(p.x()) + ", " + QString::number(p.y()) + ")";
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
    qDebug("from == " + image(from));
    qDebug("to   == " + image(to));
    qDebug("closestGridPoint(from) == " + image(grid.closestGridPoint(from)));
    qDebug("closestGridPoint(to)   == " + image(grid.closestGridPoint(to)));
    qDebug("move(from, " + QString::number(x) + ", " +
	   QString::number(y) + " ) == " + image(grid.move(from, x, y)));
    qDebug("move((0, 0), 1, 1) == " + image(grid.move(QPoint(0, 0), 1, 1)));
    
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
    QPoint second = list->front();
    list->pop_front();
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
		ConnectorView *newOne = new ConnectorView(from_, to_,
							  first,
							  second,
							  canvas());
		newOne->setNextPin(current->next_.pin);
		newOne->setPrevConnector(current);
		current->setNextConnector(newOne);
		current = newOne;
	    }
	}
    }
    current->destroySuccessors();
    if (current->next_.pin != targetPin) {
	current->setNextPin(targetPin);
    }
    delete list;
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
}


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
        .arg((unsigned)to()->pinModel()->bits());
}


PinView *ConnectorView::to()
{
    return to_;
}


PinView *ConnectorView::from()
{
    return from_;
}

void ConnectorView::deleteView()
{
    delete this;
}
