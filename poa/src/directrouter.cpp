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
 * $Id: directrouter.cpp,v 1.5 2004/02/11 09:51:39 squig Exp $
 *
 *****************************************************************************/


#include <qvaluelist.h>

#include "connectorviewlist.h"
#include "directrouter.h"
#include "pinview.h"
#include "grid.h"


void DirectRouter::route(ConnectorViewList *view)
{
    PinView *source = view->source();
    Q_ASSERT(source != 0);
    PinView *target = view->target();
    Q_ASSERT(target != 0);
    QPoint sourcePoint = source->connectorPoint();
    QPoint targetPoint = target->connectorPoint();
    LineDirection sourceDir = source->connectorSourceDir();
    LineDirection targetDir = target->connectorTargetDir();
    QValueList<QPoint> *points = routeOne(sourcePoint,
                      sourceDir,
                      targetPoint,
                      targetDir);
    view->applyPointList(*points);
    delete points;
}


void DirectRouter::route(QValueList<ConnectorViewList*>& list)
{
    for (QValueList<ConnectorViewList*>::iterator it = list.begin();
     it != list.end(); ++it) {

    route(*it);
    }
}


QValueList<QPoint> *DirectRouter::routeOne(QPoint from,
                       LineDirection fromDir,
                       QPoint to,
                       LineDirection toDir)
{
    int x;
    int y;
    Grid grid(from, to, 10);
    grid.getGridDistance(from, to, x, y);
    //Q_ASSERT(grid.move(from, x, y) == to);
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

    if (fromDir == toDir) {
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
            next = grid.move(next, reverse(fromDir), QABS(fromDist) + 2);
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
            next = grid.move(next, reverse(fromDir), QABS(fromDist) + 2);
            list->append(next);
            next = grid.move(next, turnRight(fromDir), 1);
            list->append(next);
        }
    }
    else if (isRightAngle(fromDir, toDir)) {
    if (fromDist >= 1 && toDir == fromAlternateDir) {
            // one bending
            next = grid.move(from, fromDir, fromDist);
            list->append(next);
    }
    else if (fromDist >= 2 && isTurn(toDir, fromAlternateDir)) {
        // bend 3 times
        next = grid.move(from, fromDir, fromDist / 2);
        list->append(next);
        next = grid.move(next, fromAlternateDir,
                 QABS(fromAlternateDist) + 1);
        list->append(next);
        next = grid.move(next, fromDir, (fromDist + 1) / 2);
        list->append(next);
        }
    else if (fromDist == 1 && isTurn(toDir, fromAlternateDir)) {
        // spiral
        next = grid.move(from, fromDir, fromDist + 1);
        list->append(next);
        next = grid.move(next, fromAlternateDir,
                 QABS(fromAlternateDist) + 1);
        list->append(next);
        next = grid.move(next, reverse(fromDir), 1);
        list->append(next);
    }
        else if (fromAlternateDist >= 2 && toDir == fromAlternateDir) {
                  // && fromDist <= 0
            // U-turn then one bending
            next = grid.move(from, fromDir, 1);
            list->append(next);
            next = grid.move(next, fromAlternateDir, fromAlternateDist / 2);
            list->append(next);
            next = grid.move(next, reverse(fromDir), QABS(fromDist) + 1);
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
            next = grid.move(next, reverse(fromDir), QABS(fromDist) + 1);
            list->append(next);
        }
        else { // 0 <= fromAlternateDist <= 1 && fromDist <= 0
            // spiral
            LineDirection dir;
            dir = reverse(fromAlternateDir);
            next = grid.move(from, fromDir, 1);
            list->append(next);
            next = grid.move(next, dir, 1);
            list->append(next);
            next = grid.move(next, reverse(fromDir), QABS(fromDist) + 1);
            list->append(next);
        }
    }
    else if (isTurn(fromDir, toDir)) {
        if (fromAlternateDist == 0 && fromDist >= 0) {
            next = grid.move(from, fromDir, 1);
            list->append(next);
            next = grid.move(next, turnLeft(fromDir), 1);
            list->append(next);
            next = grid.move(next, fromDir, fromDist);
            list->append(next);
            next = grid.move(next, turnRight(fromDir), 1);
            list->append(next);
        }
    else if (fromAlternateDist == 0) {
         // && fromDist < 0
        next = grid.move(from, fromDir, 1);
        list->append(next);
        next = grid.move(next, turnLeft(fromDir), 1);
        list->append(next);
        next = grid.move(next, reverse(fromDir), (QABS(fromDist) + 1) / 2);
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
