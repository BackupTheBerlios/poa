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
 * $Id: pinlistviewitem.cpp,v 1.2 2003/12/17 15:58:45 garbeam Exp $
 *
 *****************************************************************************/

#include <qlistview.h>

#include "pinlistviewitem.h"

PinListViewItem::PinListViewItem(QListView *parent,
                                 QListViewItem *after,
                                 PinModel::PinType type)
    : QListViewItem(parent, after)
{
    setOpen(true);
    type_ = type;
    clone_ = origin_ = 0;
}

PinListViewItem::PinListViewItem(QListView *parent, QListViewItem *after,
                                 PinModel *clone, PinModel *origin)
    : QListViewItem(parent, after)
{
    setOpen(false);
    type_ = clone->type();
    clone_ = clone;
    origin_ = origin;

    update();
}

PinListViewItem::PinListViewItem(QListViewItem *parent,
                                 QListViewItem *after,
                                 PinModel *clone, PinModel *origin)
    : QListViewItem(parent, after)
{
    setOpen(false);
    type_ = clone->type();
    clone_ = clone;
    origin_ = origin;

    update();
}

void PinListViewItem::update() {

    if (clone_ != 0) {
        setText(0, QString::number(clone_->position(), 10));
        setText(1, clone_->name());
        if (clone_->type() != PinModel::EPISODIC) {
            setText(2, QString::number(clone_->address(), 16));
        }
        setText(3, QString::number(clone_->bits(), 10));
    }
}

PinListViewItem::~PinListViewItem()
{
    if (clone_ != 0) {
        delete clone_;
    }
}

int PinListViewItem::compare(QListViewItem *i, int col, bool ascending ) const {
    if (col == 0) { // first colum
        PinModel *nextPin = ((PinListViewItem *)i)->data();
        if (nextPin && clone_)  {
            if (ascending) {
                return clone_->position() - nextPin->position();
            }
            else {
                return nextPin->position() - clone_->position();
            }
        }
        else if (clone_) {
            return clone_->position();
        }
        else {
            return 0;
        }
    }
    else {
        return key(col, ascending).compare(i->key(col, ascending));
    }
}


PinModel *PinListViewItem::data() const
{
    return clone_;
}

PinModel *PinListViewItem::origData() const
{
    return origin_;
}

void PinListViewItem::setOrigPin(PinModel *pin) {
    origin_ = pin;
}

PinModel::PinType PinListViewItem::type() {
    return type_;
}

bool PinListViewItem::isRoot() {
    return isOpen();
}

