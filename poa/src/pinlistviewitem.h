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
 * $Id: pinlistviewitem.h,v 1.8 2004/01/29 15:15:36 squig Exp $
 *
 *****************************************************************************/

#ifndef POA_PINLISTVIEWITEM_H
#define POA_PINLISTVIEWITEM_H

#include "pinmodel.h"

class QListViewItem;

/**
 * Provides the I/O view items.
 */
class PinListViewItem : public QListViewItem
{
public:

    /**
     * Creates a PinListViewItem for the given data.
     */
    PinListViewItem(QListView *parent, QListViewItem *after,
                    PinModel::PinType type);

    /**
     * Creates a PinListViewItem for the given data.
     */
    PinListViewItem(QListViewItem *parent, QListViewItem *after,
                    PinModel::PinType type, PinModel *pin);

    /**
     * Default destructor
     */
    ~PinListViewItem();

    /**
     * Returns true, if item is root item like "Input Pins" and
     * therefore does not store a pin.
     */
    bool isRoot();

    /**
     * Returns the PinModel, represented by this view item
     */
    PinModel *pin() const;

    /**
     * Returns type of this view item.
     */
    PinModel::PinType type();

    /**
     * Creates a new PinModel from this item and sets the pin to the
     * newly created one.
     */
    PinModel *createPin(BlockModel* block);

    /**
     * Cancels any renaming.
     */
    void cancelRename();

    /**
     * Commits all changes to the dedicated PinModel or just
     * creates a new one, if this is a new pin.
     */
    void commit() const;

    virtual void okRename(int col);

    /**
     * Updates the view with current contents of the pin model.
     */
    void update();

    /** Overloaded compare method. */
    int compare(QListViewItem *i, int col, bool ascending ) const;

    void flushSortOrder();

private:

    PinModel *pin_;
    PinModel::PinType type_;
    bool root_;
};

#endif // POA_PINLISTVIEWITEM_H
