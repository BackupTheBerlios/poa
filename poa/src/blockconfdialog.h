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
 * $Id: blockconfdialog.h,v 1.4 2003/09/11 17:53:41 garbeam Exp $
 *
 *****************************************************************************/

#ifndef BLOCKCONFDIALOG_H
#define BLOCKCONFDIALOG_H

#include "blockmodel.h"
#include "pinmodel.h"

#include <qlistview.h>
#include <qvariant.h>
#include <qdialog.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QButtonGroup;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListView;
class QPushButton;
class QRadioButton;
class QSpinBox;

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
    PinListViewItem(QListViewItem *parent, PinModel *item = 0);

    /**
     * Default destructor
     */
    ~PinListViewItem();

    /**
     * Returns the PinModel, represented by this view item
     */
    PinModel *data() const;

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

    /**
     * Updates the view with current contents of the pin model.
     */
    void update();

private:
    PinModel *item_;
    PinModel::PinType type_;
    bool root_;
};


class BlockConfDialog : public QDialog
{ 
    Q_OBJECT

public:
    BlockConfDialog(BlockModel *model, QWidget* parent = 0,
                    const char* name = 0, bool modal = FALSE, WFlags fl = 0);
    ~BlockConfDialog();

    QListView *ioListView;

    QRadioButton *offsetAutoCalcRadioButton;
    QRadioButton *offsetRadioButton;
    QRadioButton *runtimeAutoCalcRadioButton;
    QRadioButton *runtimeRadioButton;

    QSpinBox *blockClockSpinBox;
    QSpinBox *offsetSpinBox;
    QSpinBox *runtimeSpinBox;

    QLineEdit *blockNameLineEdit;
    QLineEdit *blockDescrLineEdit;
    QLineEdit *ioNumberLineEdit;
    QLineEdit *ioNameLineEdit;
    QLineEdit *addressLineEdit;
    QLineEdit *bitsLineEdit;

private:
    QPushButton *applyPushButton;
    QPushButton *compilePushButton;
    QPushButton *editCodePushButton;
    QPushButton *helpPushButton;
    QPushButton *cancelPushButton;
    QPushButton *newIoPushButton;
    QPushButton *updateIoPushButton;
    QPushButton *removeIoPushButton;
    QPushButton *okPushButton;

    PinListViewItem *inputRoot;
    PinListViewItem *outputRoot;
    PinListViewItem *episodicRoot;

    BlockModel *model_;

    /**
     * Synchronizes dialog with current model_ content.
     */
    void update();

private slots:
    /**
     * Adds a new I/O item to the dedicated I/O group.
     */
    void newIo();

    /**
     * Updates the currently selected I/O item with 
     * modified values, BUT without any semantical check, yet.
     */
    void updateIo();

    /**
     * Removes the selected I/O item from the list view.
     */
    void removeIo();

    /**
     * Disables/enables I/O manipulation buttons, if
     * a list view item was (de)selected.
     */
    void ioSelectionChanged();

    /**
     * Disables/enables offset spin box, if the manual
     * offset setting is enabled.
     */
    void toggleManualOffset();

    /**
     * Disables/enables runtime spin box, if the manual
     * runtime setting is enabled.
     */
    void toggleManualRuntime();
};

#endif // BLOCKCONFDIALOG_H
