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
 * $Id: connectorview.cpp,v 1.2 2003/08/29 17:53:19 keulsn Exp $
 *
 *****************************************************************************/


#include "connectorview.h"


ConnectorView::ConnectorView(ConnectorModel *model,
			     PinView *from,
			     PinView *to,
			     QCanvas *canvas)
    : QCanvasLine(canvas)
{
    model_ = model;
    bool changeDirection;
    bool positiveDirection;
    PinView::DockPosition dock = from->dockPosition();

    switch (dock) {
    case PinView::PIN_LEFT:
	changeDirection = (to->connectorPoint().x()
			   >= from->connectorPoint().x());
	setOrientation(HORIZONTAL);
	positiveDirection = false;
	break;
    case PinView::PIN_RIGHT:
	changeDirection = (to->connectorPoint().x()
			   <= from->connectorPoint().x());
	setOrientation(HORIZONTAL);
	positiveDirection = true;
	break;
    case PinView::PIN_TOP:
	changeDirection = (to->connectorPoint().y()
			   >= from->connectorPoint().y());
	setOrientation(VERTICAL);
	positiveDirection = false;
	break;
    case PinView::PIN_BOTTOM:
	changeDirection = (to->connectorPoint().y()
			   <= from->connectorPoint().y());
	setOrientation(VERTICAL);
	positiveDirection = true;
	break;
    }
    dockToSource(from);

    if (changeDirection) {
	// must add a short line, then turn around and find a way towards
	// the pin
	QPoint nextPoint = startPoint();
	if (orientation_ == HORIZONTAL) {
	    if (positiveDirection) {
		nextPoint.setX(nextPoint.x() + DEFAULT_DOCK_LINE_LENGTH);
	    } else {
		nextPoint.setX(nextPoint.x() - DEFAULT_DOCK_LINE_LENGTH);
	    }
	} else {
	    if (positiveDirection) {
		nextPoint.setY(nextPoint.y() + DEFAULT_DOCK_LINE_LENGTH);
	    } else {
		nextPoint.setY(nextPoint.y() - DEFAULT_DOCK_LINE_LENGTH);
	    }
	}
	
	ConnectorView *nextSegment =
	    new ConnectorView(nextPoint,
			      inflection(orientation_),
			      to,
			      canvas);
	dockToTarget(nextSegment);
    } else {
	// find inflection point
	QPoint point = firstInflectionPoint(startPoint(),
					    orientation_,
					    to->connectorPoint(),
					    to->dockPosition());
	if (point == to->connectorPoint()) {
	    // pin is reached directly
	    dockToTarget(to);
	} else {
	    // add inflection point
	    ConnectorView *nextSegment =
		new ConnectorView(point,
				  inflection(orientation_),
				  to,
				  canvas);
	    dockToTarget(nextSegment);
	}
    }
}

ConnectorView::ConnectorView(QPoint start,
			     LineOrientation orientation,
			     PinView *to,
			     QCanvas *canvas)
    : QCanvasLine(canvas)
{
    model_ = 0;
    orientation_ = orientation;
    first_ = true;
    prev_.pin = 0;
    setStartPoint(start);

    QPoint point = firstInflectionPoint(startPoint(),
					orientation_,
					to->connectorPoint(),
					to->dockPosition());
    if (point == to->connectorPoint()) {
	// pin is reached directly
	dockToTarget(to);
    } else {
	// add inflection point
	ConnectorView *nextSegment =
	    new ConnectorView(point,
			      inflection(orientation_),
			      to,
			      canvas);
	dockToTarget(nextSegment);
    }
}

ConnectorView::~ConnectorView()
{
}

ConnectorModel *ConnectorView::model()
{
    return model_;
}

QCanvasItemList ConnectorView::allSegments()
{
    QCanvasItemList list;
    ConnectorView *current = this;

    while (current != 0) {
	list.prepend(this);
	if (current->last_) {
	    current = 0;
	} else {
	    current = current->next_.connector;
	}
    }
    return list;
}

ConnectorView::LineOrientation ConnectorView::inflection
                                                  (LineOrientation orientation)
{
    switch (orientation) {
    case HORIZONTAL:
	return VERTICAL;
    case VERTICAL:
	return HORIZONTAL;
    case UNKNOWN:
	return UNKNOWN;
    default:
	// never happens, avoid compiler warning
	Q_ASSERT(false);
	return UNKNOWN;
    }
}

void ConnectorView::setStartPoint(QPoint start)
{
    QPoint end = endPoint();
    setPoints(start.x(), start.y(), end.x(), end.y());
}

void ConnectorView::setEndPoint(QPoint end)
{
    QPoint start = startPoint();
    setPoints(start.x(), start.y(), end.x(), end.y());
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


void ConnectorView::dockToSource(PinView *source)
{
    setPrevPin(source);

    int x;
    int y;
    int oldX = endPoint().x();
    int oldY = endPoint().y();
    QPoint sourcePoint = source->connectorPoint();

    switch (source->dockPosition()) {
    case PinView::PIN_LEFT:
	x = sourcePoint.x() - 1;
	y = sourcePoint.y();
	Q_ASSERT(orientation_ == HORIZONTAL);
	break;
    case PinView::PIN_RIGHT:
	x = sourcePoint.x() + 1;
	y = sourcePoint.y();
	Q_ASSERT(orientation_ == HORIZONTAL);
	break;
    case PinView::PIN_TOP:
	x = sourcePoint.x();
	y = sourcePoint.y() - 1;
	Q_ASSERT(orientation_ == VERTICAL);
	break;
    case PinView::PIN_BOTTOM:
	x = sourcePoint.x();
	y = sourcePoint.y() + 1;
	Q_ASSERT(orientation_ == VERTICAL);
	break;
    }
    if (orientation_ == HORIZONTAL) {
	setPoints(x, y, oldX, y);
    } else if (orientation_ == VERTICAL) {
	setPoints(x, y, x, oldY);
    } else {
	setPoints(x, y, x, y);
    }
}

void ConnectorView::dockToSource(ConnectorView *from)
{
    Q_ASSERT(orientation_ != UNKNOWN);
    this->setPrevConnector(from);
    from->setNextConnector(this);
    Q_ASSERT((orientation_ == HORIZONTAL
	      && this->endPoint().y() == from->endPoint().y())
	     || (orientation_ == VERTICAL
		 && this->endPoint().x() == from->endPoint().x()));
    setStartPoint(from->endPoint());
}

void ConnectorView::dockToTarget(PinView *target)
{
    setNextPin(target);

    int x;
    int y;
    int oldX = endPoint().x();
    int oldY = endPoint().y();
    QPoint targetPoint = target->connectorPoint();

    switch (target->dockPosition()) {
    case PinView::PIN_LEFT:
	x = targetPoint.x() - 1;
	y = targetPoint.y();
	Q_ASSERT(orientation_ == HORIZONTAL);
	break;
    case PinView::PIN_RIGHT:
	x = targetPoint.x() + 1;
	y = targetPoint.y();
	Q_ASSERT(orientation_ == HORIZONTAL);
	break;
    case PinView::PIN_TOP:
	x = targetPoint.x();
	y = targetPoint.y() - 1;
	Q_ASSERT(orientation_ == VERTICAL);
	break;
    case PinView::PIN_BOTTOM:
	x = targetPoint.x();
	y = targetPoint.y() + 1;
	Q_ASSERT(orientation_ == VERTICAL);
	break;
    }

    if (orientation_ == HORIZONTAL) {
	setPoints(oldX, y, x, y);
    } else if (orientation_ == VERTICAL) {
	setPoints(x, oldY, x, y);
    } else {
	setPoints(x, y, x, y);
    }
}

void ConnectorView::dockToTarget(ConnectorView *to)
{
    Q_ASSERT(orientation_ != UNKNOWN);
    this->setNextConnector(to);
    to->setPrevConnector(this);
    Q_ASSERT((orientation_ == HORIZONTAL
	      && this->startPoint().y() == to->startPoint().y())
	     || (orientation_ == VERTICAL
		 && this->startPoint().x() == to->endPoint().x()));
    setEndPoint(to->startPoint());
}

void ConnectorView::setOrientation(LineOrientation orientation)
{
    orientation_ = orientation;
}

QPoint ConnectorView::firstInflectionPoint(QPoint start,
					   LineOrientation orientation,
					   QPoint end,
					   PinView::DockPosition dock)
{
    LineOrientation targetOrientation;
    switch (dock) {
    case PinView::PIN_LEFT:
    case PinView::PIN_RIGHT:
	targetOrientation = HORIZONTAL;
	break;
    case PinView::PIN_TOP:
    case PinView::PIN_BOTTOM:
	targetOrientation = VERTICAL;
	break;
    default:
	// never happens
	targetOrientation = UNKNOWN;
	break;
    }

    if (orientation == HORIZONTAL) {
	if (targetOrientation == HORIZONTAL) {
	    if (start.y() == end.y()) {
		return end;
	    } else {
		return QPoint((start.x() + end.x()) / 2, start.y());
	    }
	} else {
	    Q_ASSERT(targetOrientation == VERTICAL);
	    return QPoint(end.x(), start.y());
	}

    } else {
	Q_ASSERT(orientation == VERTICAL);
	if (targetOrientation == HORIZONTAL) {
	    return QPoint(start.x(), end.y());
	} else {
	    Q_ASSERT(targetOrientation == VERTICAL);
	    if (start.x() == end.x()) {
		return end;
	    } else {
		return QPoint(start.x(), (start.y() + end.y()) / 2);
	    }
	}
    }
}
