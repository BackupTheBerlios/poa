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
 * $Id: muxconfdialog.h,v 1.21 2004/01/17 12:47:14 garbeam Exp $
 *
 *****************************************************************************/

#ifndef POA_MUXCONFDIALOG_H
#define POA_MUXCONFDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
#include <qlistview.h>
#include <qptrlist.h>

#include "pinmodel.h"
#include "muxmodel.h"

class PinListViewItem;

class QBoxLayout;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QRadioButton;

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

    /** Finds and returns PinModel by id. */
    PinModel *findPinById(PinModel::PinType, unsigned id);

    /** Finds and returns PinListViewItem by PinModel. */
    PinListViewItem *findPinListViewItemByPinModel(PinModel *pin);

    /** Rearranges all positions. */
    void updatePositions(QListView *lv);

    /** Updates all mappings after an I/O has been updated. */
    void updateMappings();

    /** Returns <code>true</code> if there's a dependent MuxMapping
        to the given PinModel. */
    bool existDependentMapping(PinModel *pin);

    /** Removes all dependent MuxMapping's. */
    void removeDependentMappings(PinModel *pin);

    QLineEdit* nameLineEdit;
    QLineEdit *inputNameLineEdit;
    QLineEdit *outputNameLineEdit;
    QPushButton *addInputPushButton;
    QPushButton *updateInputPushButton_;
    QPushButton *removeInputPushButton_;
    QPushButton *addOutputPushButton;
    QPushButton *updateOutputPushButton_;
    QPushButton *removeOutputPushButton_;
    QPushButton* helpPushButton;
    QPushButton* okPushButton;
    QPushButton* cancelPushButton;
    QPushButton* applyPushButton;
    QPushButton* addMappingPushButton_;
    QPushButton* updateMappingPushButton_;
    QPushButton* removeMappingPushButton_;
    QSpinBox *firstInputBitSpinBox;
    QSpinBox *lastInputBitSpinBox;
    QSpinBox *firstOutputBitSpinBox;
    QSpinBox *lastOutputBitSpinBox;
    QSpinBox *inputBitsSpinBox;
    QSpinBox *outputBitsSpinBox;
    QListView* mappingListView;
    QListView* inputListView;
    QListView* outputListView;
    QBoxLayout *dialogLayout_;

    MuxModel *model_;

    QPtrList<PinModel> deletedPins_;
    QPtrList<PinModel> updatedPins_;
    QPtrList<PinModel> newPins_;

    QPtrList<MuxMapping> deletedMappings_;
    QPtrList<MuxMapping> updatedMappings_;
    QPtrList<MuxMapping> newMappings_;

private slots:

    /** Magic selection, button title changing. */
    void listViewSelectionChanged();
    void mappingSelectionChanged();

    /** Adds a new MuxMapping. */
    void addMapping();

    /** Remove the selected MuxMapping. */
    void removeMapping();

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

    /** Adds a new input. */
    void addInput();

    /** Updates the selected input. */
    void updateInput();

    /** Removes the selected input. */
    void removeInput();

    /** Adds a new output. */
    void addOutput();

    /** Updates the selected output. */
    void updateOutput();

    /** Removes the selected output. */
    void removeOutput();
};

#endif // POA_MUXCONFDIALOG_H
