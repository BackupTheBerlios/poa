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
 * $Id: connectorviewlist.cpp,v 1.7 2003/09/21 21:05:51 vanto Exp $
 *
 *****************************************************************************/


#include "connectorviewlist.h"

#include <qcanvas.h>

#include "blockmodel.h"
#include "connectorviewsegment.h"
#include "grid.h"
#include "gridcanvas.h"
#include "pinmodel.h"

ConnectorViewList::ConnectorViewList(PinView *source,
                                     PinView *target,
                                     GridCanvas *canvas)
{
    source_ = source;
    target_ = target;

    connect(source->pinModel(), SIGNAL(deleted()),
            this, SLOT(deleteView()));
    connect(target->pinModel(), SIGNAL(deleted()),
            this, SLOT(deleteView()));

    QValueList<QPoint> *points = routeConnector(source->connectorPoint(),
                                                source->connectorDirection(),
                                                target->connectorPoint(),
                                                target->connectorDirection());
    applyPointList(*points, canvas);
    delete points;
}

ConnectorViewList::ConnectorViewList(PinView *source,
                                     PinView *target,
                                     const QValueList<QPoint> &points,
                                     GridCanvas *canvas)
{
    source_ = source;
    target_ = target;

    connect(source->pinModel(), SIGNAL(deleted()),
            this, SLOT(deleteView()));
    connect(target->pinModel(), SIGNAL(deleted()),
            this, SLOT(deleteView()));

    applyPointList(points, canvas);
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

void ConnectorViewList::serialize()
{
}

void ConnectorViewList::applyPointList(const QValueList<QPoint> &list,
                                       QCanvas *canvas)
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
            current = new ConnectorViewSegment(first, second, canvas, this);
            connect(this, SIGNAL(select(bool)), current, SLOT(select(bool)));
            it = segments_.insert(it, current);
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

QString image(QPoint p)
{
    return "(" + QString::number(p.x()) + ", " + QString::number(p.y()) + ")";
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
        else if (fromAlternateDist >= 2) { // && fromDist <= 0
            // U-turn then one bending
            next = grid.move(from, fromDir, 1);
            list->append(next);
            next = grid.move(next, fromAlternateDir, fromAlternateDist / 2);
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
            if (QABS(fromDist) >= 0) {
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
