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
 * $Id: pinlistviewitem.cpp,v 1.10 2004/01/29 15:15:36 squig Exp $
 *
 *****************************************************************************/

#include <qlistview.h>

#include "pinlistviewitem.h"

PinListViewItem::PinListViewItem(QListView *parent,
                                 QListViewItem *after,
                                 PinModel::PinType type)
    : QListViewItem(parent, after)
{
    type_ = type;
    pin_ = 0;
    root_ = true;

    setOpen(true);
}

PinListViewItem::PinListViewItem(QListViewItem *parent, QListViewItem *after,
                                 PinModel::PinType type, PinModel *pin)
    : QListViewItem(parent, after)
{
    type_ = type;
    pin_ = pin;
    root_ = false;

    setOpen(false);
    setRenameEnabled(1, true);
    setRenameEnabled(2, true);
    if (type_ != PinModel::EPISODIC) {
        setRenameEnabled(3, true);
    }

    if (pin_ != 0) {
        setText(0, QString::number(pin_->position(), 10));
        setText(1, pin_->name());
        setText(2, QString::number(pin_->bits(), 10));
        if (type_ != PinModel::EPISODIC) {
            setText(3, QString::number(pin_->address(), 16));
        }
    }
}

PinListViewItem::~PinListViewItem()
{
}

int PinListViewItem::compare(QListViewItem *next,
                             int col, bool ascending) const
{
    if (col == 0 && !isOpen()) { // first colum
        if (next)  {
            if (ascending) {
                return text(0).toInt() - next->text(0).toInt();
            }
            else {
                return next->text(0).toInt() - text(0).toInt();
            }
        }
        else {
            return text(0).toInt();
        }
    }
    else {
        return key(col, ascending).compare(next->key(col, ascending));
    }
}


PinModel *PinListViewItem::pin() const
{
    return pin_;
}

void PinListViewItem::cancelRename()
{
    // just pass 0, the parameter is ignored by qt anyways
    QListViewItem::cancelRename(0);
}

void PinListViewItem::commit() const
{
    Q_ASSERT(pin_ != 0);

    bool ok;
    int value = text(0).toUInt(&ok, 10);
    if (ok) {
        pin_->setPosition(value);
    }
    pin_->setName(text(1));
    value = text(2).toUInt(&ok, 10);
    if (ok) {
        pin_->setBits(value);
    }
    value = text(3).toUInt(&ok, 16);
    if (ok) {
        pin_->setAddress(value);
    }
}

PinModel *PinListViewItem::createPin(BlockModel* block)
{
    pin_ = new PinModel(block, text(1));
    pin_->setType(type_);
    commit();
    return pin_;
}

void PinListViewItem::flushSortOrder()
{
    enforceSortOrder();
}

bool PinListViewItem::isRoot()
{
    return root_;
}

void PinListViewItem::okRename(int col)
{
    QListViewItem::okRename(col);

    if (col < 3) {
        startRename(col + 1);
    }
}

PinModel::PinType PinListViewItem::type()
{
    return type_;
}
