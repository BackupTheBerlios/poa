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
 * $Id: reachedpoint.cpp,v 1.1 2003/09/26 14:19:00 keulsn Exp $
 *
 *****************************************************************************/


#include "reachedpoint.h"


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

QPoint Node::point() const
{
    return point_;
}

LineDirection Node::dir() const
{
    return dir_;
}

ReachedPoint::ReachedPoint(Node node,
			   unsigned badness,
			   ReachedPoint *predecessor)
    : node_(node)
{
    badness_ = badness;
    predecessor_ = predecessor;
}

void ReachedPoint::setPredecessor(ReachedPoint *predecessor)
{
    predecessor_ = predecessor;
}

ReachedPoint *ReachedPoint::predecessor() const
{
    return predecessor_;
}


void ReachedPoint::setBadness(unsigned badness)
{
    badness_ = badness;
}

unsigned ReachedPoint::badness() const
{
    return badness_;
}

bool ReachedPoint::sameNode(const ReachedPoint *other) const
{
    return this->node_ == other->node_;
}

bool ReachedPoint::betterThan(const ReachedPoint *other) const
{
    return this->badness_ < other->badness_;
}
