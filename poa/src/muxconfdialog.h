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
 * $Id: muxconfdialog.h,v 1.16 2003/12/17 15:58:45 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_MUXCONFDIALOG_H
#define POA_MUXCONFDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include <qlistview.h>
#include <qptrlist.h>

#include "pinmodel.h"

class MuxMapping;
class MuxModel;
class PinListViewItem;

class QBoxLayout;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;

/**
 * Provides the MuxMapping view items.
 */
class MuxMappingListViewItem : public QListViewItem
{
public:

    /**
     * Creates a MuxMapping list view item for the given abstract model
     */
    MuxMappingListViewItem(QListView *parent, QListViewItem *after,
                           MuxMapping *clone = 0, MuxMapping *origin = 0);

    /**
     * Default destructor
     */
    ~MuxMappingListViewItem();

    /**
     * Returns the MuxMapping, represented by this view item
     */
    MuxMapping *data() const;

    /**
     * Returns the origin MuxMapping, represented by this view item
     */
    MuxMapping *origData() const;

    /**
     * Sets original mapping.
     */
    void setOrigMapping(MuxMapping *mapping);

    /**
     * Updates the view with current contents of the MuxMapping.
     */
    void update();

private:
    MuxMapping *clone_;
    MuxMapping *origin_;
};

///////////////////////////////////////////////////////////////////////////////


class MuxConfDialog : public QDialog
{
    Q_OBJECT

public:
    MuxConfDialog(MuxModel *model, QWidget* parent = 0,
                  const char* name = 0, bool modal = FALSE, WFlags fl = 0);
    ~MuxConfDialog();

private:
    /** Initializes all Layouts */
    void initLayout();

    /** Initializes top widget. */
    void initTopWidget();

    /** Initializes I/O widget. */
    void initIOWidget();

    /** Initializes mapping widget. */
    void initMappingWidget();

    /** Initializes bottom buttons. */
    void initBottomWidget();

    /** Initializes connections. */
    void initConnections();

    /** Syncs this dialog with <code>model_</code>. */
    void syncModel();

    /** Syncs the <code>model_</code> with this dialog. */
    void updateModel();

    /** Adds a new MuxMapping to the selected item. */
    void addMapping();

    /** Adds a new IO to the mappings list view. */
    void addIo();

    /** Removes the IO given by <code>item</code>. */
    void removeIo();

    /** Removes the Mapping given by <code>item</code>. */
    void removeMapping(MuxMappingListViewItem *item);

    /** Finds and returns PinModel by id. */
    PinModel *findPinById(PinModel::PinType, unsigned id);

    QLineEdit* nameLineEdit;
    QLineEdit *ioNameLineEdit;
    QPushButton *addIoPushButton;
    QPushButton *removeIoPushButton;
    QPushButton* helpPushButton;
    QPushButton* okPushButton;
    QPushButton* cancelPushButton;
    QPushButton* applyPushButton;
    QPushButton* addPushButton;
    QPushButton* updatePushButton;
    QPushButton* removePushButton;
    QSpinBox *firstInputBitSpinBox;
    QSpinBox *lastInputBitSpinBox;
    QSpinBox *firstOutputBitSpinBox;
    QSpinBox *lastOutputBitSpinBox;
    QSpinBox *ioBitsSpinBox;
    QListView* mappingListView;
    QListView* inputListView;
    QListView* outputListView;
    QBoxLayout *dialogLayout_;

    MuxModel *model_;

    QPtrList<PinModel> deletedPins_;

private slots:

    /** Magic selection, button title changing. */
    void mappingSelectionChanged();

    /**
     * Removes a selected IO with its mappings or a selected mapping.
     */
    void removeIoOrMapping();

    /**
     * Updates a mapping.
     */
    void updateMapping();

    /**
     * Applies the changes to the model.
     */
    void apply();

    /**
     * Applies the changes to the model and closes this dialog.
     */
    void ok();

};

#endif // POA_MUXCONFDIALOG_H
