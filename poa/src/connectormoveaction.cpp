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
 * $Id: connectormoveaction.cpp,v 1.3 2004/01/28 18:00:56 squig Exp $
 *
 *****************************************************************************/

#include <qwmatrix.h>
#include <cmath>

#include "canvasview.h"
#include "connectormoveaction.h"
#include "gridcanvas.h"
#include "settings.h"

ConnectorMoveAction::ConnectorMoveAction(CanvasView *view, QMouseEvent *e, ConnectorViewSegment *item)
    : CanvasViewAction(view), item_(item)
{
    startPoint_ = view->inverseWorldMatrix().map(e->pos());
}

// Convert cartesian coordinates into an angle (DEG)
int getAngle(int x, int y)
{
        if (x == 0) return 90;

        return (int)(atan(y / x) * 360 / 2 * M_PI);
}

QPoint ConnectorMoveAction::dragBy(int dx, int dy)
{
        // Horizontal lines can only be moved vertically and vice versa, so set
        // the other delta component to 0.
        int xs = QABS(item_->endPoint().x() - item_->startPoint().x());
        int ys = QABS(item_->endPoint().y() - item_->startPoint().y());
        if (xs > ys) dx = 0;
        if (ys > xs) dy = 0;

        QPoint startP = item_->startPoint();
        QPoint endP = item_->endPoint();
        QValueList<QPoint> newPoints;
        QValueList<QPoint> points = item_->viewList()->points();

        // Copy old point list into new and move our start and end-point.
        // Create new point if needed (segment is on a pin).
    QValueList<QPoint>::Iterator it;
    QValueList<QPoint>::Iterator lastIt = points.begin();
    for (it = points.begin(); it != points.end(); ++it ) {
                if ((*it == startP) || (*it == endP)) {
                        // Translate point using delta
                        QPoint newPoint = QPoint((*it).x() + dx, (*it).y() + dy);

                        // Point was first in list (= connects to pin), so first create
                        // another point that still connects to the pin
                        if (*it == *points.begin())
                                newPoints.append(*it);
                        newPoints.append(newPoint);             // Insert translated point
                        if (*it == *(--points.end()))   // Same as before, but last in list
                                newPoints.append(*it);

                } else
                        newPoints.append(*it);                  // Just copy all other points

                lastIt = it;
    }

        // Update our start and end points (later used to re-find our segment)
        startP.rx() += dx;  startP.ry() += dy;
        endP.rx() += dx;        endP.ry() += dy;

        // Clean up unneccesary points (points that lie on an line)
        // We use the line angle to check this
        int angle, lastAngle;
        int state = 0;          // "State machine", helps our initialisation process
        QPoint firstPoint;
    for (it = newPoints.begin();it != newPoints.end(); ++it ) {
                state++;
                if (state > 1) {
                        // If this is a dublicate point, remove it
                        if (*it == *lastIt) {
                                newPoints.remove(it);
                                it = lastIt;            // it got invalid after the remove
                                continue;
                        }
                        angle = getAngle((*lastIt).x() - (*it).x(), (*lastIt).y() - (*it).y());

                        if ((state > 2) && (lastAngle == angle)) {
                                // Check if the point to be removed is one of our segment
                                // and replace with point before/after if necessary
                                if (*lastIt == startP)  startP = firstPoint;
                                if (*lastIt == endP)    endP = *it;
                                newPoints.remove(lastIt);
                        } else
                                // First point of line we're currently following
                                firstPoint = *lastIt;
                }
                lastAngle = angle;
                lastIt = it;
        }

        // Save viewList now, item_ might become invalid after applyPointList
        ConnectorViewList *viewList = item_->viewList();
        viewList->applyPointList(newPoints);

        // Now look through new segments list and search for "our" segment
        QCanvasItemList itemList = viewList->allSegments();
        QCanvasItemList::iterator itemIt;
        for (itemIt = itemList.begin(); itemIt != itemList.end(); ++itemIt) {
                if ((((ConnectorViewSegment*)(*itemIt))->startPoint() == startP) &&
                        (((ConnectorViewSegment*)(*itemIt))->endPoint() == endP))
                {
                        item_ = (ConnectorViewSegment*)*itemIt;         // Got it
                        break;
                }
        }

        // Re-select all segments (in case some new ones had to be created)
        viewList->setSelected(true);
    return QPoint(dx, dy);
}

void ConnectorMoveAction::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p = view()->inverseWorldMatrix().map(e->pos());
    int dx = 0;
    int dy = 0;

    if (Settings::instance()->snapToGrid() &&
        !(e->stateAfter() & Qt::ControlButton)) {
        QPoint oldPos = QPoint((int)item_->x(), (int)item_->y());

        // move to closest grid coordinate
        QPoint newPos = view()->gridCanvas()->toGrid(oldPos + p - startPoint_);

        dx = newPos.x() - oldPos.x();
        dy = newPos.y() - oldPos.y();
    }
    else {
        dx = p.x() - startPoint_.x();
        dy = p.y() - startPoint_.y();
    }

    // make sure item is not moved outside of canvas
        int minX = QMIN(item_->startPoint().x(), item_->endPoint().x());
    if (minX + dx < 0) {
        dx = -minX;
    }
        int minY = QMIN(item_->startPoint().y(), item_->endPoint().y());
    if (minY + dy < 0) {
        dy = -minY;
    }
        int maxX = QMAX(item_->startPoint().x(), item_->endPoint().x());
    if (maxX + dx > view()->canvas()->width()) {
        dx = view()->canvas()->width() - maxX;
    }
        int maxY = QMAX(item_->startPoint().y(), item_->endPoint().y());
    if (maxY + dy > view()->canvas()->height()) {
        dy = view()->canvas()->height() - maxY;
    }

    if (dx != 0 || dy != 0) {
        QPoint moved = dragBy(dx, dy);
        startPoint_ += moved;

        view()->gridCanvas()->ensureVisibility(item_);
        view()->canvas()->update();
    }
}

void ConnectorMoveAction::mouseReleaseEvent(QMouseEvent *)
{
    view()->setAction(0);
}
