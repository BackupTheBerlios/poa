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
 * $Id: pinlistviewitem.h,v 1.4 2004/01/20 17:59:42 squig Exp $
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
     * Creates a IO list view item for the given abstract model
     */
    PinListViewItem(QListView *parent, QListViewItem *after = 0,
                    PinModel::PinType type = PinModel::INPUT);

    /**
     * Creates a IO list view item for the given abstract model
     */
    PinListViewItem(QListView *parent, QListViewItem *after,
                    PinModel *clone = 0, PinModel *origin = 0);

    /**
     * Creates a IO list view item for the given abstract model
     */
    PinListViewItem(QListViewItem *parent, QListViewItem *after,
                    PinModel *clone = 0, PinModel *origin = 0);

    /**
     * Default destructor
     */
    ~PinListViewItem();

    /**
     * Returns the PinModel, represented by this view item
     */
    PinModel *data() const;

    /**
     * Returns the origin PinModel, represented by this view item
     */
    PinModel *origData() const;

    /**
     * Sets the original pin.
     */
    void PinListViewItem::setOrigPin(PinModel *pin);

    /**
     * Returns type of this view item (used for items without
     * models like root items.
     */
    PinModel::PinType type();

    /**
     * Returns <code>true</code>, if this is a root item,
     * <code>false</code> otherwise.
     */
    bool isRoot();

    virtual void okRename(int col);

    /**
     * Updates the view with current contents of the pin model.
     */
    void update();

    /** Overloaded compare method. */
    int compare(QListViewItem *i, int col, bool ascending ) const;

private:

    void initialize();

    PinModel *clone_;
    PinModel *origin_;
    PinModel::PinType type_;
    bool root_;

};

#endif // POA_PINLISTVIEWITEM_H
