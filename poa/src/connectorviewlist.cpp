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
 * $Id: connectorviewlist.cpp,v 1.19 2003/12/03 16:06:20 squig Exp $
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



ConnectorViewList::ConnectorViewList(PinView *source,
                                     PinView *target,
                                     GridCanvas *canvas,
                                     QDomElement *element)
{
    Q_ASSERT(source->canvas() == canvas);
    Q_ASSERT(target->canvas() == canvas);

    source_ = source;
    target_ = target;
    canvas_ = canvas;

    segments_.setAutoDelete(true);

    source_->setConnector(this);
    target_->setConnector(this);

    if (element != 0) {
        deserialize(element);
    }
}

ConnectorViewList::~ConnectorViewList()
{
    segments_.clear();

    source_->setConnector(0);
    target_->setConnector(0);
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
    QCanvasItemList list;
    for (ConnectorViewSegment *current = segments_.first();
     current != 0; current = segments_.next()) {

    list.prepend(current);
    }
    return list;
}

void ConnectorViewList::deleteSegment(ConnectorViewSegment *seg)
{
    Q_ASSERT(segments_.contains(seg));
    segments_.remove(seg);

    // if list is empty, delete connection
    if (segments_.isEmpty()) {
        Q_ASSERT(source_ != 0 && source_->pinModel() != 0);
        Q_ASSERT(target_ != 0 && target_->pinModel() != 0);

        source_->pinModel()->detach();
        // the call above will detach the target pin as well, so we do
        // not need to do it
        // target_->pinModel()->detach();
    }
}

QValueList<QPoint> ConnectorViewList::points()
{
    QValueList<QPoint> list;
    QValueList<QPoint>::iterator point = list.end();
    ConnectorViewSegment *current = segments_.first();
    while (current != 0) {
        list.insert(point, current->startPoint());
        ++point;
        current = segments_.next();
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
    ConnectorViewSegment *current = segments_.first();
    while (current != 0) {
        current->select(selected);
        current = segments_.next();
    }
}

void ConnectorViewList::setVisible(bool)
{
    // do nothing, the view list is never displayed
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
    if (list.size() < 2) {
    segments_.clear();
    }
    else {

    QValueList<QPoint>::const_iterator point = list.begin();
    ConnectorViewSegment *current = segments_.first();

    QPoint second = *point;
    ++point;
    QPoint first;

    while (point != list.end()) {
        first = second;
        second = *point;
        ++point;

        if (current != 0) {
        current->setPoints(first.x(), first.y(),
                   second.x(), second.y());
        }
        else {
        current = new ConnectorViewSegment(first, second,
                           canvas_, this);

        segments_.append(current);
        current->show();
        }
        current = segments_.next();
    }

    if (current != 0) {
        while (current != segments_.last()) {
        segments_.removeLast();
        }
        segments_.removeLast();
    }
    }
}

/*
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
}*/
