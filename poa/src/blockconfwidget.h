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
 * $Id: blockconfwidget.h,v 1.7 2004/01/29 14:27:22 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_BLOCKCONFWIDGET_H
#define POA_BLOCKCONFWIDGET_H

#include <qlistview.h>
#include <qptrlist.h>
#include <qwidget.h>

#include "pinmodel.h"

class BlockModel;
class PinListViewItem;

class QListView;
class QListViewItem;
class QPoint;
class QPushButton;

/**
 * Block Configuration Widget.
 */
class BlockConfWidget : public QWidget
{
    Q_OBJECT

public:
    BlockConfWidget(BlockModel *model, QWidget* parent = 0);
    ~BlockConfWidget();

    /**
     * Syncs model with this widget.
     */
    void sync();

    /**
     * Commits all changes to the model.
     */
    void commit();

    QListView *ioListView() const;

    /**
     * Returns PinListViewItem for a given PinModel, if it exists,
     * otherwise <code>0</code>.
     */
    PinListViewItem *pinListViewItemForPin(PinModel *pin);

private:
    QListView *ioListView_;

    PinListViewItem *inputRoot_;
    PinListViewItem *outputRoot_;
    PinListViewItem *episodicRoot_;

    BlockModel *model_;
    QPtrList<PinModel> deletedPins_;

    QPushButton *newIoPushButton_;
    QPushButton *removeIoPushButton_;
    bool newPinSortOrder_;

    PinListViewItem *currentRenameItem;

    void cancelRename();

    /**
     * Initializes layout.
     */
    void initLayout();

    /**
     * Updates all positions.
     */
    void updatePositions(PinModel::PinType type);

    /** Swaps two list view items of the same type. */
    void swapItems(PinListViewItem *item1,
                   PinListViewItem *item2);

private slots:

    void mouseButtonClicked(int button, QListViewItem *item,
                            const QPoint & pos, int c);

    /**
     * Adds a new I/O item to the dedicated I/O group.
     */
    void newIo();

    /**
     * Removes the selected I/O item from the list view.
     */
    void removeIo();

    /** Enables/disables new/remove buttons. */
    void ioSelectionChanged();

    /**
     * Moves a list view row upwards.
     */
    void moveRowUp();

    /**
     * Moves a list view row downwards.
     */
    void moveRowDown();
};

class IOListView : public QListView
{
public:

    IOListView(QWidget *parent = 0, const char *name = 0, WFlags f = 0)
        : QListView(parent, name, f) {};

protected:

    void keyPressEvent(QKeyEvent *e)
        {
            QListView::keyPressEvent(e);

            if (e->key() == Key_F2) {
                if (currentItem() && currentItem()->renameEnabled(1)) {
                    currentItem()->startRename(1);
                }
            }
        };
};

#endif // POA_BLOCKCONFWIDGET_H
