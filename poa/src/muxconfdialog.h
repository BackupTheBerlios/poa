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
 * $Id: muxconfdialog.h,v 1.25 2004/01/29 14:27:22 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_MUXCONFDIALOG_H
#define POA_MUXCONFDIALOG_H

#include <qdialog.h>
#include <qlistview.h>
#include <qptrlist.h>

#include "pinmodel.h"
#include "muxmodel.h"

class BlockConfWidget;
class PinListViewItem;

class QBoxLayout;
class QLineEdit;
class QPushButton;

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
                           PinListViewItem *input,
                           PinListViewItem *output,
                           MuxMapping *mapping = 0);

    /**
     * Default destructor
     */
    ~MuxMappingListViewItem();

    /**
     * Returns the MuxMapping, represented by this view item
     */
    MuxMapping *mapping() const;

    /**
     * Updates the view with current contents of the MuxMapping.
     */
    void update();

    /** I/O handling. */
    void setInputPinListViewItem(PinListViewItem *input);
    void setOutputPinListViewItem(PinListViewItem *output);

    PinListViewItem *inputPinListViewItem();
    PinListViewItem *outputPinListViewItem();

    /* Ranges. */
    void setFirstInputBit(unsigned firstInputBit);
    void setFirstOutputBit(unsigned firstOutputBit);
    void setLastInputBit(unsigned lastInputBit);
    void setLastOutputBit(unsigned lastOutputBit);

    unsigned firstInputBit();
    unsigned firstOutputBit();
    unsigned lastInputBit();
    unsigned lastOutputBit();

private:
    MuxMapping *mapping_;
    PinListViewItem *input_;
    PinListViewItem *output_;
    unsigned firstInputBit_;
    unsigned firstOutputBit_;
    unsigned lastInputBit_;
    unsigned lastOutputBit_;
};

///////////////////////////////////////////////////////////////////////////////


/**
 * Mux Configuration Dialog.
 */
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

    /** Initializes mapping widget. */
    void initMappingWidget();

    /** Initializes bottom buttons. */
    void initBottomWidget();

    /** Initializes connections. */
    void initConnections();

    /** Syncs this dialog with <code>model_</code>. */
    void sync();

    /** Commits all changes to the <code>model_</code>. */
    void commit();

    /** Updates all mappings after an I/O has been updated. */
    void updateMappings();

    /** Returns <code>true</code> if there's a dependent MuxMapping
        to the given PinModel. */
    bool existDependentMapping(PinModel *pin);

    /** Removes all dependent MuxMapping's. */
    void removeDependentMappings(PinModel *pin);

    QLineEdit* nameLineEdit;
    QPushButton* helpPushButton;
    QPushButton* okPushButton;
    QPushButton* cancelPushButton;
    QPushButton* applyPushButton;
    QPushButton *newMappingButton;
    QPushButton *updateMappingButton;
    QPushButton *removeMappingButton;
    QListView* mappingListView;
    QBoxLayout *dialogLayout_;

    MuxModel *model_;
    BlockConfWidget *blockConfWidget_;

    QPtrList<MuxMapping> deletedMappings_;

private slots:

    /** Magic selection, button title changing. */
    void mappingSelectionChanged();

    /** Adds a new MuxMapping. */
    void addMapping();

    /** Updates a MuxMapping. */
    void updateMapping();

    /** Remove the selected MuxMapping. */
    void removeMapping();

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
